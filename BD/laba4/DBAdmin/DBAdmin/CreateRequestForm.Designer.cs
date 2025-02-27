namespace DBAdmin
{
    partial class CreateRequestForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            panel1 = new Panel();
            CreateRequestBtn = new Button();
            AddResidentBtn = new Button();
            AddApartmentBtn = new Button();
            AddUserBtn = new Button();
            ResidentInput = new ComboBox();
            label4 = new Label();
            CommentsInput = new RichTextBox();
            label3 = new Label();
            ApartmentInput = new ComboBox();
            label2 = new Label();
            ClientInput = new ComboBox();
            label1 = new Label();
            panel1.SuspendLayout();
            SuspendLayout();
            // 
            // panel1
            // 
            panel1.Controls.Add(CreateRequestBtn);
            panel1.Controls.Add(AddResidentBtn);
            panel1.Controls.Add(AddApartmentBtn);
            panel1.Controls.Add(AddUserBtn);
            panel1.Controls.Add(ResidentInput);
            panel1.Controls.Add(label4);
            panel1.Controls.Add(CommentsInput);
            panel1.Controls.Add(label3);
            panel1.Controls.Add(ApartmentInput);
            panel1.Controls.Add(label2);
            panel1.Controls.Add(ClientInput);
            panel1.Controls.Add(label1);
            panel1.Location = new Point(27, 12);
            panel1.Name = "panel1";
            panel1.Size = new Size(411, 516);
            panel1.TabIndex = 0;
            // 
            // CreateRequestBtn
            // 
            CreateRequestBtn.BackColor = Color.Teal;
            CreateRequestBtn.Font = new Font("Segoe UI", 15.75F, FontStyle.Bold, GraphicsUnit.Point, 204);
            CreateRequestBtn.ForeColor = SystemColors.ButtonHighlight;
            CreateRequestBtn.Location = new Point(119, 472);
            CreateRequestBtn.Name = "CreateRequestBtn";
            CreateRequestBtn.Size = new Size(156, 41);
            CreateRequestBtn.TabIndex = 12;
            CreateRequestBtn.Text = "Создать";
            CreateRequestBtn.UseVisualStyleBackColor = false;
            CreateRequestBtn.Click += CreateRequestBtn_Click;
            // 
            // AddResidentBtn
            // 
            AddResidentBtn.BackColor = Color.Green;
            AddResidentBtn.FlatAppearance.BorderColor = Color.Green;
            AddResidentBtn.FlatAppearance.BorderSize = 0;
            AddResidentBtn.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point, 204);
            AddResidentBtn.ForeColor = SystemColors.ButtonFace;
            AddResidentBtn.Location = new Point(3, 252);
            AddResidentBtn.Name = "AddResidentBtn";
            AddResidentBtn.Size = new Size(126, 30);
            AddResidentBtn.TabIndex = 11;
            AddResidentBtn.Text = "Добавить";
            AddResidentBtn.UseVisualStyleBackColor = false;
            AddResidentBtn.Click += AddResidentBtn_Click;
            // 
            // AddApartmentBtn
            // 
            AddApartmentBtn.BackColor = Color.Green;
            AddApartmentBtn.FlatAppearance.BorderColor = Color.Green;
            AddApartmentBtn.FlatAppearance.BorderSize = 0;
            AddApartmentBtn.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point, 204);
            AddApartmentBtn.ForeColor = SystemColors.ButtonFace;
            AddApartmentBtn.Location = new Point(3, 152);
            AddApartmentBtn.Name = "AddApartmentBtn";
            AddApartmentBtn.Size = new Size(126, 30);
            AddApartmentBtn.TabIndex = 10;
            AddApartmentBtn.Text = "Добавить";
            AddApartmentBtn.UseVisualStyleBackColor = false;
            AddApartmentBtn.Click += AddApartmentBtn_Click;
            // 
            // AddUserBtn
            // 
            AddUserBtn.BackColor = Color.Green;
            AddUserBtn.FlatAppearance.BorderColor = Color.Green;
            AddUserBtn.FlatAppearance.BorderSize = 0;
            AddUserBtn.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point, 204);
            AddUserBtn.ForeColor = SystemColors.ButtonFace;
            AddUserBtn.Location = new Point(3, 51);
            AddUserBtn.Name = "AddUserBtn";
            AddUserBtn.Size = new Size(126, 30);
            AddUserBtn.TabIndex = 9;
            AddUserBtn.Text = "Добавить";
            AddUserBtn.UseVisualStyleBackColor = false;
            AddUserBtn.Click += AddUserBtn_Click;
            // 
            // ResidentInput
            // 
            ResidentInput.DisplayMember = "Name";
            ResidentInput.FormattingEnabled = true;
            ResidentInput.Location = new Point(3, 223);
            ResidentInput.Name = "ResidentInput";
            ResidentInput.Size = new Size(405, 23);
            ResidentInput.TabIndex = 8;
            ResidentInput.ValueMember = "Id";
            ResidentInput.KeyUp += ResidentInput_KeyUp;
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(0, 298);
            label4.Name = "label4";
            label4.Size = new Size(84, 15);
            label4.TabIndex = 7;
            label4.Text = "Комментарий";
            // 
            // CommentsInput
            // 
            CommentsInput.Location = new Point(0, 316);
            CommentsInput.MaxLength = 150;
            CommentsInput.Name = "CommentsInput";
            CommentsInput.Size = new Size(405, 47);
            CommentsInput.TabIndex = 6;
            CommentsInput.Text = "";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(3, 205);
            label3.Name = "label3";
            label3.Size = new Size(79, 15);
            label3.TabIndex = 4;
            label3.Text = "Собственник";
            // 
            // ApartmentInput
            // 
            ApartmentInput.DisplayMember = "Address";
            ApartmentInput.FormattingEnabled = true;
            ApartmentInput.Location = new Point(3, 123);
            ApartmentInput.Name = "ApartmentInput";
            ApartmentInput.Size = new Size(405, 23);
            ApartmentInput.TabIndex = 3;
            ApartmentInput.ValueMember = "Id";
            ApartmentInput.KeyUp += ApartmentInput_KeyUp;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(3, 105);
            label2.Name = "label2";
            label2.Size = new Size(40, 15);
            label2.TabIndex = 2;
            label2.Text = "Адрес";
            // 
            // ClientInput
            // 
            ClientInput.DisplayMember = "Name";
            ClientInput.FormattingEnabled = true;
            ClientInput.Location = new Point(3, 22);
            ClientInput.Name = "ClientInput";
            ClientInput.Size = new Size(405, 23);
            ClientInput.TabIndex = 1;
            ClientInput.Tag = "Id";
            ClientInput.ValueMember = "Id";
            ClientInput.KeyUp += ClientInput_KeyUp;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(3, 4);
            label1.Name = "label1";
            label1.Size = new Size(78, 15);
            label1.TabIndex = 1;
            label1.Text = "Имя клиента";
            // 
            // CreateRequestForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(461, 540);
            Controls.Add(panel1);
            Name = "CreateRequestForm";
            Text = "Создать заявку";
            Load += CreateRequestForm_Load;
            panel1.ResumeLayout(false);
            panel1.PerformLayout();
            ResumeLayout(false);
        }

        #endregion

        private Panel panel1;
        private Label label1;
        private ComboBox ClientInput;
        private ComboBox ApartmentInput;
        private Label label2;
        private Label label3;
        private RichTextBox CommentsInput;
        private Label label4;
        private ComboBox ResidentInput;
        private Button AddUserBtn;
        private Button AddApartmentBtn;
        private Button AddResidentBtn;
        private Button CreateRequestBtn;
    }
}