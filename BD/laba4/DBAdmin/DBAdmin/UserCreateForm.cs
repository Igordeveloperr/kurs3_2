using DBAdmin.context;
using DBAdmin.DTO;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DBAdmin
{
    public partial class UserCreateForm : Form
    {
        private const int MIN_NAME_LEN = 6;
        private const int MIN_PASSWORD_LEN = 10;

        public UserCreateForm()
        {
            InitializeComponent();
        }

        private void UserCreateForm_Load(object sender, EventArgs e)
        {

        }


        // создаем пользователя запрос к БД
        private void CreateUserBtn_Click(object sender, EventArgs e)
        {
            ErrorDTO err1 = ValidateUserName();
            ErrorDTO err2 = ValidatePassword();

            if (!err1.Result && !err2.Result)
            {
                AddUserToDb();
            }
            else if (err1.Result)
            {
                MessageBox.Show(err1.Message);
            }
            else if (err2.Result)
            {
                MessageBox.Show(err2.Message);
            }
        }

        // сам запрос к БД
        private void AddUserToDb()
        {
            using (AppDbContext db = new AppDbContext())
            {
                var exUser = db.Users.Where(x => x.Name == UserNameInput.Text)
                    .ToList()
                    .FirstOrDefault();
                if(exUser == null)
                {
                    db.Users.Add(new entity.User
                    {
                        Name = UserNameInput.Text,
                        Password = UserPasswordInput.Text
                    });
                    db.SaveChanges();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Такой пользователь уже существует!");
                }
            }
        }

        // проверка имени пользователя
        private ErrorDTO ValidateUserName()
        {
            ErrorDTO dto = new ErrorDTO();
            if (UserNameInput.Text.Length < MIN_NAME_LEN) {
                dto.Result = true;
                dto.Message = $"Минимальная длина имени {MIN_NAME_LEN} символов!";
            }
            else if (string.IsNullOrWhiteSpace(UserNameInput.Text))
            {
                dto.Result = true;
                dto.Message = "Имя не может быть пустым!";
            }
            return dto;
        }

        // проверка пароля
        private ErrorDTO ValidatePassword()
        {
            ErrorDTO dto = new ErrorDTO();
            if (UserPasswordInput.Text.Length < MIN_PASSWORD_LEN)
            {
                dto.Result = true;
                dto.Message = $"Минимальная длина пароля {MIN_PASSWORD_LEN} символов!";
            }
            else if (string.IsNullOrWhiteSpace(UserPasswordInput.Text))
            {
                dto.Result = true;
                dto.Message = "Пароль не может быть пустым!";
            }
            return dto;
        }
    }
}
