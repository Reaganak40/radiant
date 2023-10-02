using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Launcher.Source
{
    enum StateOptions
    {
        WelcomeScreen,
        CreateProject,
    }

    internal class State
    {
        

        private StateOptions m_state;

        public State()
        {
            m_state = StateOptions.WelcomeScreen;
        }

        public void SetState(StateOptions state)
        {
            m_state = state;
        }

        public string GetStateStr()
        {
            switch(m_state) 
            {
                case StateOptions.WelcomeScreen:
                    return "Radiant > Welcome";
                case StateOptions.CreateProject:
                    return "Radiant > Welcome > Create Project";
                default:
                    return "Radiant >";
            }
        }
    }
}
