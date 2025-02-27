namespace DBAdmin
{
    partial class ApartmentCreateForm
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
            label2 = new Label();
            StreetInput = new TextBox();
            label1 = new Label();
            CityInput = new TextBox();
            label3 = new Label();
            HouseInput = new TextBox();
            CreateApartmentBtn = new Button();
            SuspendLayout();
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(35, 69);
            label2.Name = "label2";
            label2.Size = new Size(41, 15);
            label2.TabIndex = 7;
            label2.Text = "Улица";
            // 
            // StreetInput
            // 
            StreetInput.Location = new Point(35, 89);
            StreetInput.MaxLength = 25;
            StreetInput.Name = "StreetInput";
            StreetInput.Size = new Size(194, 23);
            StreetInput.TabIndex = 6;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(35, 11);
            label1.Name = "label1";
            label1.Size = new Size(40, 15);
            label1.TabIndex = 5;
            label1.Text = "Город";
            // 
            // CityInput
            // 
            CityInput.Location = new Point(35, 31);
            CityInput.MaxLength = 25;
            CityInput.Name = "CityInput";
            CityInput.Size = new Size(194, 23);
            CityInput.TabIndex = 4;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(35, 127);
            label3.Name = "label3";
            label3.Size = new Size(76, 15);
            label3.TabIndex = 9;
            label3.Text = "Номер дома";
            // 
            // HouseInput
            // 
            HouseInput.Location = new Point(35, 147);
            HouseInput.MaxLength = 25;
            HouseInput.Name = "HouseInput";
            HouseInput.Size = new Size(194, 23);
            HouseInput.TabIndex = 8;
            HouseInput.KeyPress += HouseInput_KeyPress;
            // 
            // CreateApartmentBtn
            // 
            CreateApartmentBtn.BackColor = Color.Teal;
            CreateApartmentBtn.Font = new Font("Segoe UI", 15.75F, FontStyle.Bold, GraphicsUnit.Point, 204);
            CreateApartmentBtn.ForeColor = SystemColors.ButtonHighlight;
            CreateApartmentBtn.Location = new Point(50, 192);
            CreateApartmentBtn.Name = "CreateApartmentBtn";
            CreateApartmentBtn.Size = new Size(156, 41);
            CreateApartmentBtn.TabIndex = 14;
            CreateApartmentBtn.Text = "Создать";
            CreateApartmentBtn.UseVisualStyleBackColor = false;
            CreateApartmentBtn.Click += CreateApartmentBtn_Click;
            // 
            // ApartmentCreateForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(263, 257);
            Controls.Add(CreateApartmentBtn);
            Controls.Add(label3);
            Controls.Add(HouseInput);
            Controls.Add(label2);
            Controls.Add(StreetInput);
            Controls.Add(label1);
            Controls.Add(CityInput);
            Name = "ApartmentCreateForm";
            Text = "Создать объект";
            Load += ApartmentCreateForm_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label label2;
        private TextBox StreetInput;
        private Label label1;
        private TextBox CityInput;
        private Label label3;
        private TextBox HouseInput;
        private Button CreateApartmentBtn;
    }
}