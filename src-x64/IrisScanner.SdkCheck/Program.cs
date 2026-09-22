using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using IrisScanner;

namespace IrisScanner.SdkCheck
{
    internal static class Program
    {
        private const int TemplateSize = 512;

        private static int Main(string[] args)
        {
            Options options;
            try
            {
                options = Options.Parse(args);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.Message);
                PrintUsage();
                return 1;
            }

            if (options.ShowHelp)
            {
                PrintUsage();
                return 0;
            }

            try
            {
                PrintTitle("M10 Iris Scanner SDK Check");
                PrintValue("Executable folder", AppDomain.CurrentDomain.BaseDirectory);
                PrintValue("Output folder", options.OutputFolder);
                PrintValue("Capture mode", options.CaptureMode.ToString());
                PrintValue("Timeout seconds", options.TimeoutSeconds.ToString());
                RequireRuntimeFile("IKConfig.ini");
                RequireRuntimeFile("IKSDKAPI.dll");
                RequireRuntimeFile("IrisScanner.Bridge.dll");

                Directory.CreateDirectory(options.OutputFolder);

                using (var bridge = new IrisScannerBridge())
                {
                    WireEvents(bridge);

                    var version = bridge.GetVersion();
                    PrintValue("SDK version", string.IsNullOrWhiteSpace(version) ? "(empty)" : version);

                    bridge.ApplySettings(new IrisScannerSettings
                    {
                        CaptureMode = options.CaptureMode,
                        EnrollSampleCount = options.EnrollSampleCount,
                        TimeoutSeconds = options.TimeoutSeconds,
                        EnforceEnroll = options.EnforceEnroll,
                        AlignmentStepValue = 35
                    });

                    bridge.Initialize(IrisInitMode.Both);
                    Console.WriteLine("Initialize => success");

                    if (options.CheckOnly)
                    {
                        bridge.Release();
                        Console.WriteLine("Check-only complete.");
                        return 0;
                    }

                    if (options.Enroll)
                    {
                        return Enroll(bridge, options);
                    }

                    if (options.Identify)
                    {
                        return Identify(bridge, options);
                    }

                    bridge.Release();
                    Console.WriteLine("No operation selected. Check-only complete.");
                    return 0;
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.GetType().Name + ": " + ex.Message);
                return 1;
            }
        }

        private static int Enroll(IrisScannerBridge bridge, Options options)
        {
            var signal = new ManualResetEventSlim(false);
            IrisEnrollCompletedEventArgs completed = null;

            bridge.EnrollCompleted += delegate (object sender, IrisEnrollCompletedEventArgs e)
            {
                completed = e;
                signal.Set();
            };

            Console.WriteLine("StartEnroll => " + options.SubjectId);
            bridge.StartEnroll(options.SubjectId);

            if (!signal.Wait(TimeSpan.FromSeconds(options.TimeoutSeconds + 10)))
            {
                bridge.StopEnroll();
                throw new TimeoutException("Timed out waiting for enrollment completion.");
            }

            bridge.Release();

            Console.WriteLine("EnrollCompleted => success=" + completed.Success + ", vendorCode=" + completed.VendorCode);
            Console.WriteLine("Message => " + completed.Message);
            var saved = SaveTemplates(options.OutputFolder, completed.Templates);
            Console.WriteLine("Saved template count => " + saved);

            return completed.Success ? 0 : 2;
        }

        private static int Identify(IrisScannerBridge bridge, Options options)
        {
            var templates = LoadTemplates(options.TemplateFolder).ToList();
            Console.WriteLine("Loaded templates => " + templates.Count);
            if (templates.Count == 0)
            {
                throw new InvalidOperationException("No .bin iris templates were found under: " + options.TemplateFolder);
            }

            var signal = new ManualResetEventSlim(false);
            IrisIdentifyCompletedEventArgs completed = null;

            bridge.IdentifyCompleted += delegate (object sender, IrisIdentifyCompletedEventArgs e)
            {
                completed = e;
                signal.Set();
            };

            Console.WriteLine("StartIdentify => " + options.TemplateFolder);
            bridge.StartIdentify(templates);

            if (!signal.Wait(TimeSpan.FromSeconds(options.TimeoutSeconds + 10)))
            {
                bridge.StopIdentify();
                throw new TimeoutException("Timed out waiting for identification completion.");
            }

            bridge.Release();

            Console.WriteLine("IdentifyCompleted => success=" + completed.Success + ", vendorCode=" + completed.VendorCode);
            Console.WriteLine("Matched subject => " + (string.IsNullOrWhiteSpace(completed.MatchedSubjectId) ? "(none)" : completed.MatchedSubjectId));
            Console.WriteLine("Matched eye => " + completed.MatchedEye);
            Console.WriteLine("Match score => " + completed.MatchScore);
            Console.WriteLine("Message => " + completed.Message);

            return completed.Success ? 0 : 3;
        }

        private static void WireEvents(IrisScannerBridge bridge)
        {
            bridge.LogReceived += delegate (object sender, ScannerLogEventArgs e)
            {
                Console.WriteLine("[LOG][" + e.Level + "][" + e.Category + "] " + e.Message);
            };

            bridge.StateUpdated += delegate (object sender, IrisStateUpdatedEventArgs e)
            {
                Console.WriteLine("[STATE] mode=" + e.WorkMode + ", left=" + e.LeftProgress.ToString("0") + "%, right=" + e.RightProgress.ToString("0") + "%, distance=" + e.Distance + ", status=" + e.StatusMessage);
            };

            bridge.IrisQualityUpdated += delegate (object sender, IrisQualityUpdatedEventArgs e)
            {
                Console.WriteLine("[QUALITY] L=" + e.LeftQualityScore + ", R=" + e.RightQualityScore + ", distance=" + e.Distance);
            };

            bridge.DeviceInfoUpdated += delegate (object sender, DeviceInfoUpdatedEventArgs e)
            {
                Console.WriteLine("[DEVICE] sn=" + e.SerialNumber + ", type=" + e.DeviceType + ", event=" + e.CameraEventCode);
            };
        }

        private static int SaveTemplates(string outputFolder, IEnumerable<IrisTemplateRecord> templates)
        {
            var saved = 0;
            var timestamp = DateTime.Now.ToString("yyyyMMdd_HHmmss_fff");

            foreach (var record in templates ?? Enumerable.Empty<IrisTemplateRecord>())
            {
                if (record == null || record.TemplateBytes == null || record.TemplateBytes.Length == 0)
                {
                    continue;
                }

                var subjectFolder = Path.Combine(outputFolder, SanitizeFileName(record.SubjectId));
                Directory.CreateDirectory(subjectFolder);

                var eyePrefix = record.Eye == IrisEye.Left ? "L" : record.Eye == IrisEye.Right ? "R" : "U";
                var baseName = eyePrefix + "_" + timestamp + "_" + saved.ToString("D2");
                var templatePath = Path.Combine(subjectFolder, baseName + ".bin");
                File.WriteAllBytes(templatePath, record.TemplateBytes);
                Console.WriteLine("Template => " + templatePath + " (" + record.TemplateBytes.Length + " bytes)");

                if (record.PreviewImage != null && record.PreviewImage.Length > 0 && record.Width > 0 && record.Height > 0)
                {
                    var imagePath = Path.Combine(subjectFolder, baseName + ".bmp");
                    SaveGrayBmp(imagePath, record.PreviewImage, record.Width, record.Height);
                    Console.WriteLine("Image => " + imagePath + " (" + new FileInfo(imagePath).Length + " bytes)");
                }

                saved++;
            }

            return saved;
        }

        private static IEnumerable<IrisTemplateRecord> LoadTemplates(string templateFolder)
        {
            if (string.IsNullOrWhiteSpace(templateFolder) || !Directory.Exists(templateFolder))
            {
                yield break;
            }

            foreach (var path in Directory.GetFiles(templateFolder, "*.bin", SearchOption.AllDirectories))
            {
                var bytes = File.ReadAllBytes(path);
                if (bytes.Length != TemplateSize)
                {
                    Console.WriteLine("Skipped template with unexpected size => " + path + " (" + bytes.Length + " bytes)");
                    continue;
                }

                var fileName = Path.GetFileNameWithoutExtension(path);
                var subjectId = new DirectoryInfo(Path.GetDirectoryName(path)).Name;
                yield return new IrisTemplateRecord
                {
                    SubjectId = subjectId,
                    Eye = InferEye(fileName),
                    TemplateBytes = bytes
                };
            }
        }

        private static IrisEye InferEye(string fileName)
        {
            if (fileName != null && fileName.StartsWith("L_", StringComparison.OrdinalIgnoreCase))
            {
                return IrisEye.Left;
            }

            if (fileName != null && fileName.StartsWith("R_", StringComparison.OrdinalIgnoreCase))
            {
                return IrisEye.Right;
            }

            return IrisEye.Unknown;
        }

        private static void SaveGrayBmp(string path, byte[] pixels, int width, int height)
        {
            if (pixels == null || pixels.Length < width * height)
            {
                return;
            }

            using (var bitmap = new Bitmap(width, height, PixelFormat.Format8bppIndexed))
            {
                var palette = bitmap.Palette;
                for (var index = 0; index < 256; index++)
                {
                    palette.Entries[index] = Color.FromArgb(index, index, index);
                }

                bitmap.Palette = palette;

                var rectangle = new Rectangle(0, 0, width, height);
                var data = bitmap.LockBits(rectangle, ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);
                try
                {
                    for (var y = 0; y < height; y++)
                    {
                        Marshal.Copy(pixels, y * width, IntPtr.Add(data.Scan0, y * data.Stride), width);
                    }
                }
                finally
                {
                    bitmap.UnlockBits(data);
                }

                bitmap.Save(path, ImageFormat.Bmp);
            }
        }

        private static string SanitizeFileName(string value)
        {
            var safe = string.IsNullOrWhiteSpace(value) ? "subject" : value.Trim();
            foreach (var invalid in Path.GetInvalidFileNameChars())
            {
                safe = safe.Replace(invalid, '_');
            }

            return safe;
        }

        private static void RequireRuntimeFile(string fileName)
        {
            var path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, fileName);
            if (!File.Exists(path))
            {
                throw new FileNotFoundException("Required runtime file was not found. Build the project so vendor SDK files are copied beside the executable.", path);
            }

            Console.WriteLine("Runtime file => " + fileName);
        }

        private static void PrintUsage()
        {
            Console.WriteLine("IrisScanner.SdkCheck");
            Console.WriteLine();
            Console.WriteLine("Usage:");
            Console.WriteLine("IrisScanner.SdkCheck.exe --check-only");
            Console.WriteLine("IrisScanner.SdkCheck.exe --enroll <subject-id> [--output <folder>] [--eye Both|Left|Right] [--count <n>] [--timeout <seconds>]");
            Console.WriteLine("IrisScanner.SdkCheck.exe --identify <template-folder> [--timeout <seconds>]");
        }

        private static void PrintTitle(string title)
        {
            Console.WriteLine(title);
            Console.WriteLine(new string('-', title.Length));
        }

        private static void PrintValue(string label, string value)
        {
            Console.WriteLine(label + " => " + (string.IsNullOrWhiteSpace(value) ? "(empty)" : value));
        }

        private sealed class Options
        {
            public bool ShowHelp { get; private set; }
            public bool CheckOnly { get; private set; }
            public bool Enroll { get; private set; }
            public bool Identify { get; private set; }
            public string SubjectId { get; private set; }
            public string TemplateFolder { get; private set; }
            public string OutputFolder { get; private set; }
            public IrisEye CaptureMode { get; private set; }
            public int EnrollSampleCount { get; private set; }
            public int TimeoutSeconds { get; private set; }
            public bool EnforceEnroll { get; private set; }

            public static Options Parse(string[] args)
            {
                var options = new Options
                {
                    CheckOnly = args == null || args.Length == 0,
                    OutputFolder = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "output"),
                    CaptureMode = IrisEye.Both,
                    EnrollSampleCount = 3,
                    TimeoutSeconds = 30
                };

                for (var index = 0; index < args.Length; index++)
                {
                    var arg = args[index];
                    if (arg == "--help" || arg == "-h" || arg == "/?")
                    {
                        options.ShowHelp = true;
                    }
                    else if (arg == "--check-only")
                    {
                        options.CheckOnly = true;
                    }
                    else if (arg == "--enroll")
                    {
                        options.Enroll = true;
                        options.CheckOnly = false;
                        options.SubjectId = ReadNext(args, ref index, arg);
                    }
                    else if (arg == "--identify")
                    {
                        options.Identify = true;
                        options.CheckOnly = false;
                        options.TemplateFolder = Path.GetFullPath(ReadNext(args, ref index, arg));
                    }
                    else if (arg == "--output")
                    {
                        options.OutputFolder = Path.GetFullPath(ReadNext(args, ref index, arg));
                    }
                    else if (arg == "--eye")
                    {
                        options.CaptureMode = ParseEye(ReadNext(args, ref index, arg));
                    }
                    else if (arg == "--count")
                    {
                        options.EnrollSampleCount = Math.Max(1, int.Parse(ReadNext(args, ref index, arg)));
                    }
                    else if (arg == "--timeout")
                    {
                        options.TimeoutSeconds = Math.Max(5, int.Parse(ReadNext(args, ref index, arg)));
                    }
                    else if (arg == "--enforce")
                    {
                        options.EnforceEnroll = true;
                    }
                    else
                    {
                        throw new ArgumentException("Unknown argument: " + arg);
                    }
                }

                if (options.Enroll && options.Identify)
                {
                    throw new ArgumentException("Choose either --enroll or --identify, not both.");
                }

                return options;
            }

            private static string ReadNext(string[] args, ref int index, string name)
            {
                if (index + 1 >= args.Length)
                {
                    throw new ArgumentException(name + " requires a value.");
                }

                index++;
                return args[index];
            }

            private static IrisEye ParseEye(string value)
            {
                if (string.Equals(value, "Left", StringComparison.OrdinalIgnoreCase))
                {
                    return IrisEye.Left;
                }

                if (string.Equals(value, "Right", StringComparison.OrdinalIgnoreCase))
                {
                    return IrisEye.Right;
                }

                if (string.Equals(value, "Both", StringComparison.OrdinalIgnoreCase))
                {
                    return IrisEye.Both;
                }

                throw new ArgumentException("--eye must be Both, Left, or Right.");
            }
        }
    }
}
