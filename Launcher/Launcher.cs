using Launcher.Source;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Threading;

namespace Launcher
{
    public partial class Launcher : Form
    {
        private State m_form_state;
        private ProjectFactory m_factory;

        public Launcher()
        {
            InitializeComponent();
            m_form_state = new State();
            m_factory = new ProjectFactory();

            ChangeState(StateOptions.WelcomeScreen);
        }



        [DllImport("DwmApi")] //System.Runtime.InteropServices
        private static extern int DwmSetWindowAttribute(IntPtr hwnd, int attr, int[] attrValue, int attrSize);

        protected override void OnHandleCreated(EventArgs e)
        {
            if (DwmSetWindowAttribute(Handle, 19, new[] { 1 }, 4) != 0)
                DwmSetWindowAttribute(Handle, 20, new[] { 1 }, 4);
        }

        [DllImport("user32")]
        private static extern IntPtr GetWindowDC(IntPtr hwnd);
        private const int WM_NCPAINT = 0x85;
        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == WM_NCPAINT && this.Focused)
            {
                var dc = GetWindowDC(Handle);
                using (Graphics g = Graphics.FromHdc(dc))
                {
                    g.DrawRectangle(Pens.Black, 0, 0, Width - 1, Height - 1);
                }
            }
        }
        private void ChangeState(StateOptions nState)
        {
            switch (nState)
            {
                case StateOptions.WelcomeScreen:
                    EnableWelcomeScreenForm();
                    break;
                case StateOptions.CreateProject:
                    EnableCreateProjectForm();
                    break;
                case StateOptions.CreatingProject:
                    EnableCreatingProjectView();
                    break;
            }

            m_form_state.SetState(nState);
            this.DirectoryLabel.Text = this.m_form_state.GetStateStr();
        }

        private void EnableCreateProjectForm()
        {
            this.NoLoadedProjectsLabel.Visible = false;
            this.ProjectCreationProgressBar.Visible = false;
            this.UpdateMessageLabel.Visible = false;

            this.ProjectPanel.Visible = true;
            this.ProjectLabel.Visible = true;

            this.CreateProjectCreateDirectorySplitContainer.Visible = true;
            this.ProjectNameBox.Visible = true;
            this.DirectoryTextbox.Visible = true;
            this.BrowseCreateDirectoryButton.Visible = true;
            this.CreateFolderCheckbox.Visible = true;
            this.BackButton.Visible = true;
            this.CreateProjectButton.Visible = true;


            this.ProjectLabel.Text = "Create Project";
        }

        private void EnableWelcomeScreenForm()
        {
            this.CreateProjectCreateDirectorySplitContainer.Visible = false;
            this.DirectoryTextbox.Visible = false;
            this.BrowseCreateDirectoryButton.Visible = false;
            this.ProjectNameBox.Visible = false;
            this.CreateFolderCheckbox.Visible = false;
            this.BackButton.Visible = false;
            this.CreateProjectButton.Visible = false;

            this.NoLoadedProjectsLabel.Visible = true;

            this.ProjectCreationProgressBar.Visible = false;
            this.UpdateMessageLabel.Visible = false;

            this.ProjectPanel.Visible = true;
            this.ProjectLabel.Visible = true;


            this.ProjectLabel.Text = "Projects";
        }

        private void EnableCreatingProjectView()
        {
            this.ProjectPanel.Visible = false;
            this.ProjectLabel.Visible = false;

            this.ProjectCreationProgressBar.Visible = true;
            this.UpdateMessageLabel.Visible = true;
        }

        private void NewProjectMenuButton_Click(object sender, EventArgs e)
        {
            ChangeState(StateOptions.CreateProject);
        }

        private void BrowseCreateDirectoryButton_Click(object sender, EventArgs e)
        {
            using (var fbd = new FolderBrowserDialog())
            {
                DialogResult result = fbd.ShowDialog();

                if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(fbd.SelectedPath))
                {
                    this.DirectoryTextbox.Text = fbd.SelectedPath;
                }
            }
        }

        private void DirectoryTextbox_TextChanged(object sender, EventArgs e)
        {
            CheckForValidProject();
        }

        private void BackButton_Click(object sender, EventArgs e)
        {
            ChangeState(StateOptions.WelcomeScreen);
        }

        private void CreateProjectButton_VisibleChanged(object sender, EventArgs e)
        {
        }

        private void CreateProjectButton_EnabledChanged(object sender, EventArgs e)
        {

        }

        private void CreateProjectButton_Paint(object sender, PaintEventArgs e)
        {
            if (!this.CreateProjectButton.Enabled)
            {
                this.CreateProjectButton.ForeColor = Color.FromArgb(100, 100, 100);
            }
            else
            {
                this.CreateProjectButton.ForeColor = SystemColors.ButtonFace;
            }

            dynamic btn = (Button)sender;
            dynamic drawBrush = new SolidBrush(btn.ForeColor);
            dynamic sf = new StringFormat
            {
                Alignment = StringAlignment.Center,
                LineAlignment = StringAlignment.Center
            };
            CreateProjectButton.Text = string.Empty;
            e.Graphics.DrawString("Create", btn.Font, drawBrush, e.ClipRectangle, sf);
            drawBrush.Dispose();
            sf.Dispose();
        }

        private void ProjectNameBox_TextChanged(object sender, EventArgs e)
        {
            CheckForValidProject();
        }

        private void CheckForValidProject()
        {
            if (this.ProjectNameBox.Text.Length == 0)
            {
                this.CreateProjectButton.Enabled = false;
                return;
            }

            if (this.DirectoryTextbox.Text.Length == 0)
            {
                this.CreateProjectButton.Enabled = false;
                return;
            }

            if (!Directory.Exists(this.DirectoryTextbox.Text))
            {
                this.CreationErrorLabel.Visible = true;
                this.CreateProjectButton.Enabled = false;
                return;
            }
            this.CreationErrorLabel.Visible = false;
            this.CreateProjectButton.Enabled = true;
        }

        private void CreateProjectButton_Click(object sender, EventArgs e)
        {
            string projectPath = this.DirectoryTextbox.Text;

            if (this.CreateFolderCheckbox.Checked)
            {
                projectPath = Path.Combine(projectPath, this.ProjectNameBox.Text);
            }

            m_factory.SetProjectName(this.ProjectNameBox.Text);
            m_factory.SetStartPoint(projectPath);
            projectCreatorWorker.RunWorkerAsync();

            ChangeState(StateOptions.CreatingProject);
        }

        private void projectCreatorWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = (BackgroundWorker)sender;
            m_factory.CreateProject(ref worker);
        }

        private void projectCreatorWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            UpdateMessageLabel.Text = e.UserState?.ToString() + " " + e.ProgressPercentage.ToString();
            ProjectCreationProgressBar.Value = e.ProgressPercentage;
        }

        private void projectCreatorWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            Application.Exit();
        }
    }
}