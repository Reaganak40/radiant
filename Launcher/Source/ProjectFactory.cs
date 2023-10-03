using Launcher.Source;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Launcher.Source
{
    internal class ProjectFactory
    {

        private string projectBaseDir;
        private string projectName;
        private string creationResourcePath;
        private string radiantSourcePath;

        public ProjectFactory()
        {

            projectBaseDir = string.Empty;
            projectName = string.Empty;
            creationResourcePath = Directory.GetCurrentDirectory();
            radiantSourcePath = creationResourcePath;

#if DEBUG
#pragma warning disable CS8602 // Dereference of a possibly null reference.
            creationResourcePath = Path.Combine(Directory.GetParent(creationResourcePath).Parent.Parent.ToString(), "Resources");
            radiantSourcePath = Directory.GetParent(creationResourcePath).Parent.ToString();

#pragma warning restore CS8602 // Dereference of a possibly null reference.
#else
            MessageBox.Show("Launcher not support in this configuration!");
            Application.Exit();
#endif
            if (!Path.Exists(creationResourcePath))
            {
                DisplayErrMsg("Could not locate resource files for project creation... '" + creationResourcePath + "'");
                Application.Exit();
            }

            if (!Path.Exists(Path.Combine(radiantSourcePath, "Radiant")))
            {
                DisplayErrMsg("Could not locate Radiant engine source files... '" + radiantSourcePath + "'");
                Application.Exit();
            }
        }

        public void SetStartPoint(string startPoint)
        {
            projectBaseDir = startPoint;
        }

        public void SetProjectName(string nProjectName)
        {
            projectName = nProjectName;
        }

        public void CreateProject(ref BackgroundWorker worker)
        {
            if (projectBaseDir == string.Empty || projectName == string.Empty)
            {
                return;
            }

            worker.ReportProgress(10, "Creating directory...");
            Directory.CreateDirectory(projectBaseDir);

            // Add Premake5 lua file for easy project generation
            string resFilepathSrc = Path.Combine(creationResourcePath, "premake5.lua");
            string resFilepathDest = Path.Combine(projectBaseDir, "premake5.lua");
            worker.ReportProgress(20, "Copying " + resFilepathSrc + " to " + resFilepathDest);
            if (!CopyFile(resFilepathSrc, resFilepathDest))
            {
                return;
            }

            // Create project container directory
            string projectContainerDir = Path.Combine(projectBaseDir, projectName);
            worker.ReportProgress(25, "Creating project container folder...");
            Directory.CreateDirectory(projectContainerDir);


            // Add lua projec file
            resFilepathSrc = Path.Combine(creationResourcePath, "project.lua");
            resFilepathDest = Path.Combine(projectContainerDir, projectName + ".lua");
            worker.ReportProgress(30, "Copying " + resFilepathSrc + " to " + resFilepathDest);
            if (!CopyFile(resFilepathSrc, resFilepathDest))
            {
                return;
            }

            // Configure premake5.lua
            radiantSourcePath = radiantSourcePath.Replace("\\", "/");
            worker.ReportProgress(35, "Configuring premake5.lua...");
            string configureFilename = Path.Combine(projectBaseDir, "premake5.lua");
            ReplaceInFile(configureFilename, "RADIANTPROJECTNAME", projectName);
            ReplaceInFile(configureFilename, "RADIANTBASEDIR", radiantSourcePath);
            while (!Path.Exists(configureFilename)) ;

            // Configure project.lua
            worker.ReportProgress(45, "Configuring project.lua...");
            configureFilename = Path.Combine(projectContainerDir, projectName + ".lua");
            ReplaceInFile(configureFilename, "RADIANTPROJECTNAME", projectName);
            ReplaceInFile(configureFilename, "RADIANTBASEDIR", radiantSourcePath);
            while (!Path.Exists(configureFilename)) ;


            // Create Source folder
            string projectSouceDir = Path.Combine(projectContainerDir, "Source");
            Directory.CreateDirectory(projectSouceDir);
            worker.ReportProgress(55, "Creating " + projectSouceDir);

            // Copy application source file
            resFilepathSrc = Path.Combine(creationResourcePath, "app_template.cpp");
            resFilepathDest = Path.Combine(projectSouceDir, projectName + ".cpp");
            worker.ReportProgress(60, "Copying " + resFilepathSrc + " to " + resFilepathDest);
            if (!CopyFile(resFilepathSrc, resFilepathDest))
            {
                return;
            }

            // Configure application source file
            worker.ReportProgress(65, "Configuring " + projectName + ".cpp ...");
            string baseClassName = Regex.Replace(projectName, @"\s+", ""); ;
            configureFilename = resFilepathDest;
            ReplaceInFile(configureFilename, "RADIANTPROJECTNAME", baseClassName);

            // Add bat project file
            resFilepathSrc = Path.Combine(creationResourcePath, "build.bat");
            resFilepathDest = Path.Combine(projectBaseDir, "build.bat");
            worker.ReportProgress(70, "Copying " + resFilepathSrc + " to " + resFilepathDest);
            if (!CopyFile(resFilepathSrc, resFilepathDest))
            {
                return;
            }

            // Configure build.bat
            worker.ReportProgress(75, "Configuring build.bat...");
            string premakeExepath = Path.Combine(radiantSourcePath, "Vendor", "bin", "premake", "premake5.exe");
            if (!Path.Exists(premakeExepath))
            {
                DisplayErrMsg("Could not find premake5 exe... " + premakeExepath);
                return;
            }
            configureFilename = resFilepathDest;
            ReplaceInFile(configureFilename, "RADIANTPREMAKE5EXE", premakeExepath);
            ReplaceInFile(configureFilename, "PROJECTBASEDIR", projectBaseDir);

            // Run premake5 to create visual studio solution and project files
            worker.ReportProgress(85, "Running premake5...");
            System.Diagnostics.Process.Start(configureFilename);

            worker.ReportProgress(100, "Opening project...");
            Thread.Sleep(750);
        }

        private bool CopyFile(string src, string dest)
        {
            if (!Path.Exists(src))
            {
                DisplayErrMsg("Could not find file ... " + src); return false;
            }
            File.Copy(src, dest, true);

            return true;
        }

       private void ReplaceInFile(string filename, string find, string replace)
        {
            string text = File.ReadAllText(filename);
            text = text.Replace(find, replace);
            File.WriteAllText(filename, text);
        }

        private void DisplayErrMsg(string msg) 
        {
            MessageBox.Show(msg, "Radiant Project Launcher",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
}

