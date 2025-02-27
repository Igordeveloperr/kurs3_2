namespace DBAdmin
{
    partial class RequestViewForm
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
            label4 = new Label();
            label3 = new Label();
            label2 = new Label();
            label1 = new Label();
            ClientInput = new TextBox();
            AddressInput = new TextBox();
            ResidentInput = new TextBox();
            Comments = new RichTextBox();
            SuspendLayout();
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(12, 101);
            label4.Name = "label4";
            label4.Size = new Size(87, 15);
            label4.TabIndex = 11;
            label4.Text = "Комментарий:";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(12, 71);
            label3.Name = "label3";
            label3.Size = new Size(82, 15);
            label3.TabIndex = 10;
            label3.Text = "Собственник:";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(12, 38);
            label2.Name = "label2";
            label2.Size = new Size(43, 15);
            label2.TabIndex = 9;
            label2.Text = "Адрес:";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(12, 9);
            label1.Name = "label1";
            label1.Size = new Size(81, 15);
            label1.TabIndex = 8;
            label1.Text = "Имя клиента:";
            // 
            // ClientInput
            // 
            ClientInput.Enabled = false;
            ClientInput.Location = new Point(99, 6);
            ClientInput.Name = "ClientInput";
            ClientInput.Size = new Size(268, 23);
            ClientInput.TabIndex = 12;
            // 
            // AddressInput
            // 
            AddressInput.Enabled = false;
            AddressInput.Location = new Point(61, 35);
            AddressInput.Name = "AddressInput";
            AddressInput.Size = new Size(306, 23);
            AddressInput.TabIndex = 13;
            // 
            // ResidentInput
            // 
            ResidentInput.Enabled = false;
            ResidentInput.Location = new Point(100, 68);
            ResidentInput.Name = "ResidentInput";
            ResidentInput.Size = new Size(267, 23);
            ResidentInput.TabIndex = 14;
            // 
            // Comments
            // 
            Comments.Enabled = false;
            Comments.Location = new Point(12, 119);
            Comments.Name = "Comments";
            Comments.Size = new Size(355, 110);
            Comments.TabIndex = 15;
            Comments.Text = "";
            // 
            // RequestViewForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(379, 247);
            Controls.Add(Comments);
            Controls.Add(ResidentInput);
            Controls.Add(AddressInput);
            Controls.Add(ClientInput);
            Controls.Add(label4);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(label1);
            Name = "RequestViewForm";
            Text = "Просмотр заявки";
            Load += RequestViewForm_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label label4;
        private Label label3;
        private Label label2;
        private Label label1;
        private TextBox ClientInput;
        private TextBox AddressInput;
        private TextBox ResidentInput;
        private RichTextBox Comments;
    }
}