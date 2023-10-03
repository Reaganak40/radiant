using Launcher.Source;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Launcher.Source
{
    internal class ProjectFactory
    {

        private string filepath;
        private string projectName;

        public ProjectFactory()
        {

            filepath = string.Empty;
            projectName = string.Empty;
        }

        public void SetStartPoint(string startPoint)
        {
            filepath = startPoint;
        }

        public void SetProjectName(string nProjectName)
        {
            projectName = nProjectName;
        }

        public void CreateProject(ref BackgroundWorker worker)
        {
            if (filepath == string.Empty || projectName == string.Empty)
            {
                return;
            }

            Directory.CreateDirectory(filepath);

            for(int i = 0; i < 4; i++)
            {
                worker.ReportProgress(i * 25, "Fake progress");
            }

            worker.ReportProgress(100, "Opening project...");
            Thread.Sleep(750);
        }
    }
}

