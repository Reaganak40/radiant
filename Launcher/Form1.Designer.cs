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
            NoLoadedProjectsLabel = new Label();
            menuStrip1 = new MenuStrip();
            NewProjectMenuButton = new ToolStripMenuItem();
            LoadProjectMenuButton = new ToolStripMenuItem();
            ProjectPanel.SuspendLayout();
            menuStrip1.SuspendLayout();
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
            ProjectPanel.Controls.Add(NoLoadedProjectsLabel);
            ProjectPanel.Controls.Add(menuStrip1);
            ProjectPanel.Location = new Point(22, 146);
            ProjectPanel.Name = "ProjectPanel";
            ProjectPanel.Size = new Size(706, 503);
            ProjectPanel.TabIndex = 2;
            // 
            // NoLoadedProjectsLabel
            // 
            NoLoadedProjectsLabel.AutoSize = true;
            NoLoadedProjectsLabel.ForeColor = SystemColors.ButtonFace;
            NoLoadedProjectsLabel.Location = new Point(203, 253);
            NoLoadedProjectsLabel.Name = "NoLoadedProjectsLabel";
            NoLoadedProjectsLabel.Size = new Size(261, 20);
            NoLoadedProjectsLabel.TabIndex = 1;
            NoLoadedProjectsLabel.Text = "There are no currently loaded projects";
            // 
            // menuStrip1
            // 
            menuStrip1.BackColor = Color.Black;
            menuStrip1.Font = new Font("Segoe UI", 9F, FontStyle.Regular, GraphicsUnit.Point);
            menuStrip1.ImageScalingSize = new Size(20, 20);
            menuStrip1.Items.AddRange(new ToolStripItem[] { NewProjectMenuButton, LoadProjectMenuButton });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(704, 28);
            menuStrip1.TabIndex = 0;
            menuStrip1.Text = "ProjectPanelMenuStrip";
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
            // Launcher
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            BackColor = Color.FromArgb(37, 37, 38);
            ClientSize = new Size(1262, 673);
            Controls.Add(ProjectPanel);
            Controls.Add(ProjectLabel);
            Controls.Add(DirectoryLabel);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            MainMenuStrip = menuStrip1;
            MaximizeBox = false;
            Name = "Launcher";
            ShowIcon = false;
            StartPosition = FormStartPosition.CenterScreen;
            ProjectPanel.ResumeLayout(false);
            ProjectPanel.PerformLayout();
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label DirectoryLabel;
        private Label ProjectLabel;
        private Panel ProjectPanel;
        private MenuStrip menuStrip1;
        private ToolStripMenuItem NewProjectMenuButton;
        private ToolStripMenuItem LoadProjectMenuButton;
        private Label NoLoadedProjectsLabel;
    }
}