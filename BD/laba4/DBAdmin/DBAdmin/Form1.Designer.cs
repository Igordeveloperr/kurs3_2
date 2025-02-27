namespace DBAdmin
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            menuStrip1 = new MenuStrip();
            создатьЗаявкуToolStripMenuItem = new ToolStripMenuItem();
            обновитьToolStripMenuItem = new ToolStripMenuItem();
            RequestTable = new DataGridView();
            ResidentName1 = new DataGridViewTextBoxColumn();
            Comments1 = new DataGridViewTextBoxColumn();
            Address1 = new DataGridViewTextBoxColumn();
            ClientName1 = new DataGridViewTextBoxColumn();
            ViewBtn = new DataGridViewButtonColumn();
            UpdateBtn = new DataGridViewButtonColumn();
            DelBtn = new DataGridViewButtonColumn();
            prevBtn = new PictureBox();
            nextBtn = new PictureBox();
            menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)RequestTable).BeginInit();
            ((System.ComponentModel.ISupportInitialize)prevBtn).BeginInit();
            ((System.ComponentModel.ISupportInitialize)nextBtn).BeginInit();
            SuspendLayout();
            // 
            // menuStrip1
            // 
            menuStrip1.Items.AddRange(new ToolStripItem[] { создатьЗаявкуToolStripMenuItem, обновитьToolStripMenuItem });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(1077, 24);
            menuStrip1.TabIndex = 0;
            menuStrip1.Text = "menuStrip1";
            // 
            // создатьЗаявкуToolStripMenuItem
            // 
            создатьЗаявкуToolStripMenuItem.Name = "создатьЗаявкуToolStripMenuItem";
            создатьЗаявкуToolStripMenuItem.Size = new Size(100, 20);
            создатьЗаявкуToolStripMenuItem.Text = "Создать заявку";
            создатьЗаявкуToolStripMenuItem.Click += CreateRequestToolStripMenuItem_Click;
            // 
            // обновитьToolStripMenuItem
            // 
            обновитьToolStripMenuItem.Name = "обновитьToolStripMenuItem";
            обновитьToolStripMenuItem.Size = new Size(115, 20);
            обновитьToolStripMenuItem.Text = "Обновить список";
            обновитьToolStripMenuItem.Click += UpdateToolStripMenuItem_Click;
            // 
            // RequestTable
            // 
            RequestTable.AllowUserToAddRows = false;
            RequestTable.AllowUserToDeleteRows = false;
            RequestTable.AllowUserToResizeColumns = false;
            RequestTable.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
            RequestTable.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
            RequestTable.BackgroundColor = SystemColors.Control;
            RequestTable.BorderStyle = BorderStyle.None;
            RequestTable.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            RequestTable.Columns.AddRange(new DataGridViewColumn[] { ResidentName1, Comments1, Address1, ClientName1, ViewBtn, UpdateBtn, DelBtn });
            RequestTable.Location = new Point(12, 27);
            RequestTable.Name = "RequestTable";
            RequestTable.ReadOnly = true;
            RequestTable.ScrollBars = ScrollBars.Vertical;
            RequestTable.Size = new Size(924, 523);
            RequestTable.TabIndex = 1;
            RequestTable.CellContentClick += RequestTable_CellContentClick;
            // 
            // ResidentName1
            // 
            ResidentName1.HeaderText = "Собственник";
            ResidentName1.Name = "ResidentName1";
            ResidentName1.ReadOnly = true;
            ResidentName1.Width = 104;
            // 
            // Comments1
            // 
            Comments1.HeaderText = "Комментарии";
            Comments1.Name = "Comments1";
            Comments1.ReadOnly = true;
            Comments1.Width = 109;
            // 
            // Address1
            // 
            Address1.HeaderText = "Адрес";
            Address1.Name = "Address1";
            Address1.ReadOnly = true;
            Address1.Width = 65;
            // 
            // ClientName1
            // 
            ClientName1.HeaderText = "Имя Клиента";
            ClientName1.Name = "ClientName1";
            ClientName1.ReadOnly = true;
            ClientName1.Width = 104;
            // 
            // ViewBtn
            // 
            ViewBtn.HeaderText = "";
            ViewBtn.MinimumWidth = 10;
            ViewBtn.Name = "ViewBtn";
            ViewBtn.ReadOnly = true;
            ViewBtn.Text = "Просмотр";
            ViewBtn.UseColumnTextForButtonValue = true;
            ViewBtn.Width = 10;
            // 
            // UpdateBtn
            // 
            UpdateBtn.HeaderText = "";
            UpdateBtn.Name = "UpdateBtn";
            UpdateBtn.ReadOnly = true;
            UpdateBtn.Text = "Редактирование";
            UpdateBtn.UseColumnTextForButtonValue = true;
            UpdateBtn.Width = 5;
            // 
            // DelBtn
            // 
            DelBtn.HeaderText = "";
            DelBtn.Name = "DelBtn";
            DelBtn.ReadOnly = true;
            DelBtn.Text = "Удалить";
            DelBtn.UseColumnTextForButtonValue = true;
            DelBtn.Width = 5;
            // 
            // prevBtn
            // 
            prevBtn.Image = (Image)resources.GetObject("prevBtn.Image");
            prevBtn.Location = new Point(942, 57);
            prevBtn.Name = "prevBtn";
            prevBtn.Size = new Size(50, 50);
            prevBtn.TabIndex = 2;
            prevBtn.TabStop = false;
            prevBtn.Click += prevBtn_Click;
            // 
            // nextBtn
            // 
            nextBtn.Image = (Image)resources.GetObject("nextBtn.Image");
            nextBtn.Location = new Point(942, 125);
            nextBtn.Name = "nextBtn";
            nextBtn.Size = new Size(50, 50);
            nextBtn.TabIndex = 3;
            nextBtn.TabStop = false;
            nextBtn.Click += nextBtn_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1077, 579);
            Controls.Add(nextBtn);
            Controls.Add(prevBtn);
            Controls.Add(RequestTable);
            Controls.Add(menuStrip1);
            MainMenuStrip = menuStrip1;
            Name = "Form1";
            Text = "Админ панель";
            Load += Form1_Load;
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)RequestTable).EndInit();
            ((System.ComponentModel.ISupportInitialize)prevBtn).EndInit();
            ((System.ComponentModel.ISupportInitialize)nextBtn).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private MenuStrip menuStrip1;
        private ToolStripMenuItem создатьЗаявкуToolStripMenuItem;
        private ToolStripMenuItem обновитьToolStripMenuItem;
        private DataGridView RequestTable;
        private DataGridViewTextBoxColumn ResidentName1;
        private DataGridViewTextBoxColumn Comments1;
        private DataGridViewTextBoxColumn Address1;
        private DataGridViewTextBoxColumn ClientName1;
        private DataGridViewButtonColumn ViewBtn;
        private DataGridViewButtonColumn UpdateBtn;
        private DataGridViewButtonColumn DelBtn;
        private PictureBox prevBtn;
        private PictureBox nextBtn;
    }
}
