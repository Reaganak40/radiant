using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Launcher.Source
{
    internal class ProjectFactory
    {
        public ProjectFactory() 
        { 
        }

        public void CreateProject(string filepath)
        {
            System.IO.Directory.CreateDirectory(filepath);
        }
    }
}
