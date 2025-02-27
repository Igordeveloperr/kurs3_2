namespace DBAdmin
{
    partial class UserCreateForm
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
            UserNameInput = new TextBox();
            label1 = new Label();
            label2 = new Label();
            UserPasswordInput = new TextBox();
            CreateUserBtn = new Button();
            SuspendLayout();
            // 
            // UserNameInput
            // 
            UserNameInput.Location = new Point(12, 29);
            UserNameInput.MaxLength = 15;
            UserNameInput.Name = "UserNameInput";
            UserNameInput.Size = new Size(189, 23);
            UserNameInput.TabIndex = 0;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(12, 9);
            label1.Name = "label1";
            label1.Size = new Size(31, 15);
            label1.TabIndex = 1;
            label1.Text = "Имя";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(12, 67);
            label2.Name = "label2";
            label2.Size = new Size(49, 15);
            label2.TabIndex = 3;
            label2.Text = "Пароль";
            // 
            // UserPasswordInput
            // 
            UserPasswordInput.Location = new Point(12, 87);
            UserPasswordInput.MaxLength = 25;
            UserPasswordInput.Name = "UserPasswordInput";
            UserPasswordInput.PasswordChar = '*';
            UserPasswordInput.Size = new Size(189, 23);
            UserPasswordInput.TabIndex = 2;
            // 
            // CreateUserBtn
            // 
            CreateUserBtn.BackColor = Color.Teal;
            CreateUserBtn.Font = new Font("Segoe UI", 15.75F, FontStyle.Bold, GraphicsUnit.Point, 204);
            CreateUserBtn.ForeColor = SystemColors.ButtonHighlight;
            CreateUserBtn.Location = new Point(26, 143);
            CreateUserBtn.Name = "CreateUserBtn";
            CreateUserBtn.Size = new Size(156, 41);
            CreateUserBtn.TabIndex = 13;
            CreateUserBtn.Text = "Создать";
            CreateUserBtn.UseVisualStyleBackColor = false;
            CreateUserBtn.Click += CreateUserBtn_Click;
            // 
            // UserCreateForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(225, 212);
            Controls.Add(CreateUserBtn);
            Controls.Add(label2);
            Controls.Add(UserPasswordInput);
            Controls.Add(label1);
            Controls.Add(UserNameInput);
            Name = "UserCreateForm";
            Text = "Добавить пользователя";
            Load += UserCreateForm_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox UserNameInput;
        private Label label1;
        private Label label2;
        private TextBox UserPasswordInput;
        private Button CreateUserBtn;
    }
}