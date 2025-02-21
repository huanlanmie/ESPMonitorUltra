namespace AIDA64InfoReader
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.btnMinimize = new System.Windows.Forms.Button();
            this.textCmdBox = new System.Windows.Forms.TextBox();
            this.notifyIcon1 = new System.Windows.Forms.NotifyIcon(this.components);
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.退出ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.btnStartAutoSend = new System.Windows.Forms.Button();
            this.btnStopAutoSend = new System.Windows.Forms.Button();
            this.textSendInterval = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBoxPortSelect = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.contextMenuStrip2 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.如何使用ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.关于ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.textSendInterval)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnMinimize
            // 
            this.btnMinimize.Location = new System.Drawing.Point(244, 127);
            this.btnMinimize.Name = "btnMinimize";
            this.btnMinimize.Size = new System.Drawing.Size(302, 62);
            this.btnMinimize.TabIndex = 0;
            this.btnMinimize.Text = "窗口最小化到托盘";
            this.btnMinimize.UseVisualStyleBackColor = true;
            this.btnMinimize.Click += new System.EventHandler(this.btnMinimize_Click);
            // 
            // textCmdBox
            // 
            this.textCmdBox.Location = new System.Drawing.Point(13, 62);
            this.textCmdBox.Multiline = true;
            this.textCmdBox.Name = "textCmdBox";
            this.textCmdBox.Size = new System.Drawing.Size(534, 59);
            this.textCmdBox.TabIndex = 1;
            // 
            // notifyIcon1
            // 
            this.notifyIcon1.BalloonTipText = "双击最大化窗口";
            this.notifyIcon1.BalloonTipTitle = "双击最大化窗口";
            this.notifyIcon1.ContextMenuStrip = this.contextMenuStrip1;
            this.notifyIcon1.Icon = ((System.Drawing.Icon)(resources.GetObject("notifyIcon1.Icon")));
            this.notifyIcon1.Text = "左键单击还原窗口";
            this.notifyIcon1.Visible = true;
            this.notifyIcon1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.notifyIcon1_MouseClick);
            this.notifyIcon1.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.notifyIcon1_MouseDoubleClick);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.退出ToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(101, 26);
            // 
            // 退出ToolStripMenuItem
            // 
            this.退出ToolStripMenuItem.Name = "退出ToolStripMenuItem";
            this.退出ToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
            this.退出ToolStripMenuItem.Text = "退出";
            this.退出ToolStripMenuItem.Click += new System.EventHandler(this.退出ToolStripMenuItem_Click);
            // 
            // btnStartAutoSend
            // 
            this.btnStartAutoSend.Location = new System.Drawing.Point(10, 127);
            this.btnStartAutoSend.Name = "btnStartAutoSend";
            this.btnStartAutoSend.Size = new System.Drawing.Size(227, 26);
            this.btnStartAutoSend.TabIndex = 5;
            this.btnStartAutoSend.Text = "开始自动发送";
            this.btnStartAutoSend.UseVisualStyleBackColor = true;
            this.btnStartAutoSend.Click += new System.EventHandler(this.btnStartAutoSend_Click);
            // 
            // btnStopAutoSend
            // 
            this.btnStopAutoSend.Location = new System.Drawing.Point(10, 162);
            this.btnStopAutoSend.Name = "btnStopAutoSend";
            this.btnStopAutoSend.Size = new System.Drawing.Size(227, 26);
            this.btnStopAutoSend.TabIndex = 6;
            this.btnStopAutoSend.Text = "停止自动发送";
            this.btnStopAutoSend.UseVisualStyleBackColor = true;
            this.btnStopAutoSend.Click += new System.EventHandler(this.btnStopAutoSend_Click);
            // 
            // textSendInterval
            // 
            this.textSendInterval.Location = new System.Drawing.Point(118, 34);
            this.textSendInterval.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.textSendInterval.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.textSendInterval.Name = "textSendInterval";
            this.textSendInterval.Size = new System.Drawing.Size(70, 21);
            this.textSendInterval.TabIndex = 7;
            this.textSendInterval.Value = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.textSendInterval.ValueChanged += new System.EventHandler(this.textSendInterval_ValueChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(10, 35);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(101, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "发送间隔(毫秒)：";
            // 
            // comboBoxPortSelect
            // 
            this.comboBoxPortSelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPortSelect.FormattingEnabled = true;
            this.comboBoxPortSelect.Location = new System.Drawing.Point(478, 33);
            this.comboBoxPortSelect.Name = "comboBoxPortSelect";
            this.comboBoxPortSelect.Size = new System.Drawing.Size(69, 20);
            this.comboBoxPortSelect.TabIndex = 9;
            this.comboBoxPortSelect.DropDown += new System.EventHandler(this.comboBoxPortSelect_DropDown);
            this.comboBoxPortSelect.SelectedIndexChanged += new System.EventHandler(this.comboBoxPortSelect_SelectedIndexChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(435, 35);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 10;
            this.label3.Text = "端口：";
            // 
            // contextMenuStrip2
            // 
            this.contextMenuStrip2.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.contextMenuStrip2.Name = "contextMenuStrip2";
            this.contextMenuStrip2.Size = new System.Drawing.Size(61, 4);
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.如何使用ToolStripMenuItem,
            this.关于ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Padding = new System.Windows.Forms.Padding(5, 2, 0, 2);
            this.menuStrip1.Size = new System.Drawing.Size(557, 25);
            this.menuStrip1.TabIndex = 12;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 如何使用ToolStripMenuItem
            // 
            this.如何使用ToolStripMenuItem.Name = "如何使用ToolStripMenuItem";
            this.如何使用ToolStripMenuItem.Size = new System.Drawing.Size(68, 21);
            this.如何使用ToolStripMenuItem.Text = "如何使用";
            this.如何使用ToolStripMenuItem.Click += new System.EventHandler(this.如何使用ToolStripMenuItem_Click);
            // 
            // 关于ToolStripMenuItem
            // 
            this.关于ToolStripMenuItem.Name = "关于ToolStripMenuItem";
            this.关于ToolStripMenuItem.Size = new System.Drawing.Size(44, 21);
            this.关于ToolStripMenuItem.Text = "关于";
            this.关于ToolStripMenuItem.Click += new System.EventHandler(this.关于ToolStripMenuItem_Click_1);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(557, 204);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.comboBoxPortSelect);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textSendInterval);
            this.Controls.Add(this.btnStopAutoSend);
            this.Controls.Add(this.btnStartAutoSend);
            this.Controls.Add(this.textCmdBox);
            this.Controls.Add(this.btnMinimize);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "AIDA64数据读取器 教程版";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.SizeChanged += new System.EventHandler(this.Form1_SizeChanged);
            this.contextMenuStrip1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.textSendInterval)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnMinimize;
        private System.Windows.Forms.TextBox textCmdBox;
        private System.Windows.Forms.NotifyIcon notifyIcon1;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 退出ToolStripMenuItem;
        private System.Windows.Forms.Button btnStartAutoSend;
        private System.Windows.Forms.Button btnStopAutoSend;
        private System.Windows.Forms.NumericUpDown textSendInterval;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox comboBoxPortSelect;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip2;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 如何使用ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 关于ToolStripMenuItem;
    }
}

