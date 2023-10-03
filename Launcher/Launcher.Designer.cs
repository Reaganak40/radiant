namespace Launcher
{
    partial class Launcher
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            DirectoryLabel = new Label();
            ProjectLabel = new Label();
            ProjectPanel = new Panel();
            CreationErrorLabel = new Label();
            BackButton = new Button();
            CreateProjectButton = new Button();
            CreateProjectCreateDirectorySplitContainer = new SplitContainer();
            DirectoryTextbox = new TextBox();
            BrowseCreateDirectoryButton = new Button();
            CreateFolderCheckbox = new CheckBox();
            NoLoadedProjectsLabel = new Label();
            ProjectPanelMenuStrip = new MenuStrip();
            NewProjectMenuButton = new ToolStripMenuItem();
            LoadProjectMenuButton = new ToolStripMenuItem();
            ProjectNameBox = new TextBox();
            ProjectCreationProgressBar = new ProgressBar();
            UpdateMessageLabel = new Label();
            projectCreatorWorker = new System.ComponentModel.BackgroundWorker();
            ProjectPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)CreateProjectCreateDirectorySplitContainer).BeginInit();
            CreateProjectCreateDirectorySplitContainer.Panel1.SuspendLayout();
            CreateProjectCreateDirectorySplitContainer.Panel2.SuspendLayout();
            CreateProjectCreateDirectorySplitContainer.SuspendLayout();
            ProjectPanelMenuStrip.SuspendLayout();
            SuspendLayout();
            // 
            // DirectoryLabel
            // 
            DirectoryLabel.AutoSize = true;
            DirectoryLabel.ForeColor = SystemColors.ButtonFace;
            DirectoryLabel.Location = new Point(12, 9);
            DirectoryLabel.Name = "DirectoryLabel";
            DirectoryLabel.Size = new Size(140, 20);
            DirectoryLabel.TabIndex = 0;
            DirectoryLabel.Text = "Radiant > Welcome";
            // 
            // ProjectLabel
            // 
            ProjectLabel.AutoSize = true;
            ProjectLabel.Font = new Font("Segoe UI", 36F, FontStyle.Regular, GraphicsUnit.Point);
            ProjectLabel.ForeColor = SystemColors.ButtonFace;
            ProjectLabel.Location = new Point(12, 44);
            ProjectLabel.Name = "ProjectLabel";
            ProjectLabel.Size = new Size(244, 81);
            ProjectLabel.TabIndex = 1;
            ProjectLabel.Text = "Projects";
            // 
            // ProjectPanel
            // 
            ProjectPanel.BackColor = Color.FromArgb(25, 25, 25);
            ProjectPanel.BorderStyle = BorderStyle.FixedSingle;
            ProjectPanel.Controls.Add(CreationErrorLabel);
            ProjectPanel.Controls.Add(BackButton);
            ProjectPanel.Controls.Add(CreateProjectButton);
            ProjectPanel.Controls.Add(CreateProjectCreateDirectorySplitContainer);
            ProjectPanel.Controls.Add(CreateFolderCheckbox);
            ProjectPanel.Controls.Add(NoLoadedProjectsLabel);
            ProjectPanel.Controls.Add(ProjectPanelMenuStrip);
            ProjectPanel.Controls.Add(ProjectNameBox);
            ProjectPanel.Location = new Point(22, 146);
            ProjectPanel.Name = "ProjectPanel";
            ProjectPanel.Size = new Size(706, 503);
            ProjectPanel.TabIndex = 2;
            // 
            // CreationErrorLabel
            // 
            CreationErrorLabel.AutoSize = true;
            CreationErrorLabel.ForeColor = Color.IndianRed;
            CreationErrorLabel.Location = new Point(24, 202);
            CreationErrorLabel.Name = "CreationErrorLabel";
            CreationErrorLabel.Size = new Size(230, 20);
            CreationErrorLabel.TabIndex = 8;
            CreationErrorLabel.Text = "* Project Directory does not exist!";
            CreationErrorLabel.Visible = false;
            // 
            // BackButton
            // 
            BackButton.FlatAppearance.BorderColor = SystemColors.WindowFrame;
            BackButton.FlatStyle = FlatStyle.Flat;
            BackButton.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point);
            BackButton.ForeColor = SystemColors.ButtonFace;
            BackButton.Location = new Point(492, 443);
            BackButton.Name = "BackButton";
            BackButton.Size = new Size(94, 45);
            BackButton.TabIndex = 7;
            BackButton.Text = "Back";
            BackButton.UseVisualStyleBackColor = true;
            BackButton.Click += BackButton_Click;
            // 
            // CreateProjectButton
            // 
            CreateProjectButton.Enabled = false;
            CreateProjectButton.FlatAppearance.BorderColor = SystemColors.WindowFrame;
            CreateProjectButton.FlatStyle = FlatStyle.Flat;
            CreateProjectButton.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point);
            CreateProjectButton.ForeColor = SystemColors.ButtonFace;
            CreateProjectButton.Location = new Point(592, 443);
            CreateProjectButton.Name = "CreateProjectButton";
            CreateProjectButton.Size = new Size(94, 45);
            CreateProjectButton.TabIndex = 6;
            CreateProjectButton.Text = "Create";
            CreateProjectButton.UseVisualStyleBackColor = true;
            CreateProjectButton.EnabledChanged += CreateProjectButton_EnabledChanged;
            CreateProjectButton.VisibleChanged += CreateProjectButton_VisibleChanged;
            CreateProjectButton.Click += CreateProjectButton_Click;
            CreateProjectButton.Paint += CreateProjectButton_Paint;
            // 
            // CreateProjectCreateDirectorySplitContainer
            // 
            CreateProjectCreateDirectorySplitContainer.Location = new Point(16, 104);
            CreateProjectCreateDirectorySplitContainer.Name = "CreateProjectCreateDirectorySplitContainer";
            // 
            // CreateProjectCreateDirectorySplitContainer.Panel1
            // 
            CreateProjectCreateDirectorySplitContainer.Panel1.Controls.Add(DirectoryTextbox);
            // 
            // CreateProjectCreateDirectorySplitContainer.Panel2
            // 
            CreateProjectCreateDirectorySplitContainer.Panel2.Controls.Add(BrowseCreateDirectoryButton);
            CreateProjectCreateDirectorySplitContainer.Size = new Size(670, 34);
            CreateProjectCreateDirectorySplitContainer.SplitterDistance = 571;
            CreateProjectCreateDirectorySplitContainer.TabIndex = 4;
            CreateProjectCreateDirectorySplitContainer.Visible = false;
            // 
            // DirectoryTextbox
            // 
            DirectoryTextbox.BackColor = Color.FromArgb(25, 25, 25);
            DirectoryTextbox.BorderStyle = BorderStyle.FixedSingle;
            DirectoryTextbox.Dock = DockStyle.Fill;
            DirectoryTextbox.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point);
            DirectoryTextbox.ForeColor = SystemColors.ButtonFace;
            DirectoryTextbox.Location = new Point(0, 0);
            DirectoryTextbox.Name = "DirectoryTextbox";
            DirectoryTextbox.PlaceholderText = "Project Directory";
            DirectoryTextbox.Size = new Size(571, 34);
            DirectoryTextbox.TabIndex = 0;
            DirectoryTextbox.TextChanged += DirectoryTextbox_TextChanged;
            // 
            // BrowseCreateDirectoryButton
            // 
            BrowseCreateDirectoryButton.BackColor = Color.FromArgb(25, 25, 25);
            BrowseCreateDirectoryButton.Dock = DockStyle.Fill;
            BrowseCreateDirectoryButton.FlatAppearance.BorderColor = SystemColors.WindowFrame;
            BrowseCreateDirectoryButton.FlatStyle = FlatStyle.Flat;
            BrowseCreateDirectoryButton.ForeColor = SystemColors.ButtonFace;
            BrowseCreateDirectoryButton.Location = new Point(0, 0);
            BrowseCreateDirectoryButton.Name = "BrowseCreateDirectoryButton";
            BrowseCreateDirectoryButton.Size = new Size(95, 34);
            BrowseCreateDirectoryButton.TabIndex = 0;
            BrowseCreateDirectoryButton.Text = "Browse";
            BrowseCreateDirectoryButton.UseVisualStyleBackColor = false;
            BrowseCreateDirectoryButton.Click += BrowseCreateDirectoryButton_Click;
            // 
            // CreateFolderCheckbox
            // 
            CreateFolderCheckbox.AutoSize = true;
            CreateFolderCheckbox.Checked = true;
            CreateFolderCheckbox.CheckState = CheckState.Checked;
            CreateFolderCheckbox.FlatAppearance.CheckedBackColor = Color.FromArgb(192, 64, 0);
            CreateFolderCheckbox.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point);
            CreateFolderCheckbox.ForeColor = SystemColors.WindowFrame;
            CreateFolderCheckbox.Location = new Point(16, 156);
            CreateFolderCheckbox.Name = "CreateFolderCheckbox";
            CreateFolderCheckbox.Size = new Size(244, 32);
            CreateFolderCheckbox.TabIndex = 5;
            CreateFolderCheckbox.Text = "Create folder for project";
            CreateFolderCheckbox.UseVisualStyleBackColor = true;
            // 
            // NoLoadedProjectsLabel
            // 
            NoLoadedProjectsLabel.AutoSize = true;
            NoLoadedProjectsLabel.ForeColor = SystemColors.ButtonFace;
            NoLoadedProjectsLabel.Location = new Point(210, 253);
            NoLoadedProjectsLabel.Name = "NoLoadedProjectsLabel";
            NoLoadedProjectsLabel.Size = new Size(261, 20);
            NoLoadedProjectsLabel.TabIndex = 1;
            NoLoadedProjectsLabel.Text = "There are no currently loaded projects";
            // 
            // ProjectPanelMenuStrip
            // 
            ProjectPanelMenuStrip.BackColor = Color.Black;
            ProjectPanelMenuStrip.Font = new Font("Segoe UI", 9F, FontStyle.Regular, GraphicsUnit.Point);
            ProjectPanelMenuStrip.ImageScalingSize = new Size(20, 20);
            ProjectPanelMenuStrip.Items.AddRange(new ToolStripItem[] { NewProjectMenuButton, LoadProjectMenuButton });
            ProjectPanelMenuStrip.Location = new Point(0, 0);
            ProjectPanelMenuStrip.Name = "ProjectPanelMenuStrip";
            ProjectPanelMenuStrip.Size = new Size(704, 28);
            ProjectPanelMenuStrip.TabIndex = 0;
            // 
            // NewProjectMenuButton
            // 
            NewProjectMenuButton.ForeColor = SystemColors.ButtonFace;
            NewProjectMenuButton.Name = "NewProjectMenuButton";
            NewProjectMenuButton.RightToLeft = RightToLeft.Yes;
            NewProjectMenuButton.Size = new Size(103, 24);
            NewProjectMenuButton.Text = "New Project";
            NewProjectMenuButton.Click += NewProjectMenuButton_Click;
            // 
            // LoadProjectMenuButton
            // 
            LoadProjectMenuButton.ForeColor = SystemColors.ButtonFace;
            LoadProjectMenuButton.Name = "LoadProjectMenuButton";
            LoadProjectMenuButton.Size = new Size(106, 24);
            LoadProjectMenuButton.Text = "Load Project";
            // 
            // ProjectNameBox
            // 
            ProjectNameBox.BackColor = Color.FromArgb(25, 25, 25);
            ProjectNameBox.BorderStyle = BorderStyle.FixedSingle;
            ProjectNameBox.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point);
            ProjectNameBox.ForeColor = SystemColors.ButtonFace;
            ProjectNameBox.Location = new Point(16, 47);
            ProjectNameBox.MaxLength = 60;
            ProjectNameBox.Name = "ProjectNameBox";
            ProjectNameBox.PlaceholderText = "Project Name";
            ProjectNameBox.Size = new Size(670, 34);
            ProjectNameBox.TabIndex = 3;
            ProjectNameBox.Visible = false;
            ProjectNameBox.TextChanged += ProjectNameBox_TextChanged;
            // 
            // ProjectCreationProgressBar
            // 
            ProjectCreationProgressBar.Location = new Point(137, 624);
            ProjectCreationProgressBar.Name = "ProjectCreationProgressBar";
            ProjectCreationProgressBar.Size = new Size(1000, 25);
            ProjectCreationProgressBar.TabIndex = 3;
            ProjectCreationProgressBar.Visible = false;
            // 
            // UpdateMessageLabel
            // 
            UpdateMessageLabel.AutoSize = true;
            UpdateMessageLabel.Font = new Font("Segoe UI", 9F, FontStyle.Regular, GraphicsUnit.Point);
            UpdateMessageLabel.ForeColor = SystemColors.ButtonFace;
            UpdateMessageLabel.Location = new Point(137, 589);
            UpdateMessageLabel.Name = "UpdateMessageLabel";
            UpdateMessageLabel.Size = new Size(160, 20);
            UpdateMessageLabel.TabIndex = 4;
            UpdateMessageLabel.Text = "Creating project files ...";
            UpdateMessageLabel.Visible = false;
            // 
            // projectCreatorWorker
            // 
            projectCreatorWorker.WorkerReportsProgress = true;
            projectCreatorWorker.DoWork += projectCreatorWorker_DoWork;
            projectCreatorWorker.ProgressChanged += projectCreatorWorker_ProgressChanged;
            projectCreatorWorker.RunWorkerCompleted += projectCreatorWorker_RunWorkerCompleted;
            // 
            // Launcher
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            BackColor = Color.FromArgb(37, 37, 38);
            ClientSize = new Size(1262, 673);
            Controls.Add(UpdateMessageLabel);
            Controls.Add(ProjectCreationProgressBar);
            Controls.Add(ProjectLabel);
            Controls.Add(DirectoryLabel);
            Controls.Add(ProjectPanel);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            MainMenuStrip = ProjectPanelMenuStrip;
            MaximizeBox = false;
            Name = "Launcher";
            ShowIcon = false;
            StartPosition = FormStartPosition.CenterScreen;
            ProjectPanel.ResumeLayout(false);
            ProjectPanel.PerformLayout();
            CreateProjectCreateDirectorySplitContainer.Panel1.ResumeLayout(false);
            CreateProjectCreateDirectorySplitContainer.Panel1.PerformLayout();
            CreateProjectCreateDirectorySplitContainer.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)CreateProjectCreateDirectorySplitContainer).EndInit();
            CreateProjectCreateDirectorySplitContainer.ResumeLayout(false);
            ProjectPanelMenuStrip.ResumeLayout(false);
            ProjectPanelMenuStrip.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label DirectoryLabel;
        private Label ProjectLabel;
        private Panel ProjectPanel;
        private MenuStrip ProjectPanelMenuStrip;
        private ToolStripMenuItem NewProjectMenuButton;
        private ToolStripMenuItem LoadProjectMenuButton;
        private Label NoLoadedProjectsLabel;
        private TextBox ProjectNameBox;
        private SplitContainer CreateProjectCreateDirectorySplitContainer;
        private TextBox DirectoryTextbox;
        private Button BrowseCreateDirectoryButton;
        private CheckBox CreateFolderCheckbox;
        private Button CreateProjectButton;
        private Button BackButton;
        private Label CreationErrorLabel;
        private ProgressBar ProjectCreationProgressBar;
        private Label UpdateMessageLabel;
        private System.ComponentModel.BackgroundWorker projectCreatorWorker;
    }
}