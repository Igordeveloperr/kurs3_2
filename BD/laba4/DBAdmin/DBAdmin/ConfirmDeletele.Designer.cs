namespace DBAdmin
{
    partial class ConfirmDeletele
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
            label1 = new Label();
            yesBtn = new Button();
            noBtn = new Button();
            SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(12, 9);
            label1.Name = "label1";
            label1.Size = new Size(240, 20);
            label1.TabIndex = 0;
            label1.Text = "Вы точно хотите удалить запись?";
            // 
            // yesBtn
            // 
            yesBtn.Location = new Point(27, 55);
            yesBtn.Name = "yesBtn";
            yesBtn.Size = new Size(94, 29);
            yesBtn.TabIndex = 1;
            yesBtn.Text = "Да";
            yesBtn.UseVisualStyleBackColor = true;
            yesBtn.Click += yesBtn_Click;
            // 
            // noBtn
            // 
            noBtn.Location = new Point(141, 55);
            noBtn.Name = "noBtn";
            noBtn.Size = new Size(94, 29);
            noBtn.TabIndex = 2;
            noBtn.Text = "Нет";
            noBtn.UseVisualStyleBackColor = true;
            noBtn.Click += noBtn_Click;
            // 
            // ConfirmDeletele
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(266, 110);
            Controls.Add(noBtn);
            Controls.Add(yesBtn);
            Controls.Add(label1);
            Name = "ConfirmDeletele";
            Text = "Внимание";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label label1;
        private Button yesBtn;
        private Button noBtn;
    }
}