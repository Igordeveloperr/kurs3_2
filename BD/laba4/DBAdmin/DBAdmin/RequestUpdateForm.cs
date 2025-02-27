using DBAdmin.context;
using DBAdmin.DTO;
using DBAdmin.entity;
using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;

namespace DBAdmin
{
    public partial class RequestUpdateForm : Form
    {
        private List<User> _users;
        private List<ApartmentBuilding> _apartments;
        private int _requestId;

        public RequestUpdateForm(int id)
        {
            InitializeComponent();
            _requestId = id;
            LoadDefaultData();
        }

        private void SetDefaultValues(int id)
        {
            using (AppDbContext db = new AppDbContext())
            {
                var application = db.RequestClientApplications
                    .Where(x => x.Id == id)
                    .Include(x => x.Apartment)
                    .Include(x => x.Client)
                    .FirstOrDefault();

                if (application != null)
                {
                    // значения по умолчанию
                    ResidentInput.SelectedIndex = _users
                        .FindIndex(user =>
                        user.Name.Equals(application.ResidentName, StringComparison.OrdinalIgnoreCase)
                        );
                    ClientInput.SelectedIndex = _users.FindIndex(x => x.Id == application.Client.Id);
                    ApartmentInput.SelectedIndex = _apartments.FindIndex(
                        x => x.Id ==
                            SelectApartmentId($"{application.Apartment.City}, {application.Apartment.Street}, д.{application.Apartment.HouseNumber}")

                    );
                }
                CommentsInput.Text = application.Comments;
            }
        }

        // поиск id объекта по адресу
        private int SelectApartmentId(string address)
        {
            int res = 1;
            foreach (var x in _apartments)
            {
                if ($"{x.City}, {x.Street}, д.{x.HouseNumber}".Equals(address))
                {
                    res = x.Id;
                }
            }

            return res;
        }

        private void LoadDefaultData()
        {
            ClientInput.Items.Clear();
            ResidentInput.Items.Clear();
            ApartmentInput.Items.Clear();

            using (AppDbContext db = new AppDbContext())
            {
                // блокировка на время подгрузки
                ClientInput.Enabled = false;
                ResidentInput.Enabled = false;
                ApartmentInput.Enabled = false;

                // запрос к БД на получение пользователей
                _users = db.Users.ToList();
                _users.ForEach(x =>
                {
                    ClientInput.Items.Add(x);
                    ResidentInput.Items.Add(x);
                });

                // запрос к БД на получение адресов
                _apartments = db.ApartmentBuildings.ToList();
                _apartments.ForEach(x =>
                {
                    ApartmentInput.Items.Add(
                        new ApartmentDTO
                        {
                            Id = x.Id,
                            Address = $"{x.City}, {x.Street}, д.{x.HouseNumber}"
                        }
                    );
                });


                SetDefaultValues(_requestId);

                // возвращение доступа к элементам
                ClientInput.Enabled = true;
                ResidentInput.Enabled = true;
                ApartmentInput.Enabled = true;
            }
        }

        private void UpdateRequestBtn_Click(object sender, EventArgs e)
        {
            using (AppDbContext db = new AppDbContext())
            {
                var application = db.RequestClientApplications
                    .Where(x => x.Id == _requestId)
                    .FirstOrDefault();
                if (application != null)
                {
                    application.ClientId = _users.Find(x => x.Name.Equals(ClientInput.Text)).Id;
                    application.ObjectTypeId = 1;
                    application.ObjectNumId = SelectApartmentId(ApartmentInput.Text);
                    application.ResidentName = ResidentInput.Text;
                    application.Comments = CommentsInput.Text;

                    db.SaveChanges();
                }
            }
            this.Close();
        }

        private void AddUserBtn_Click(object sender, EventArgs e)
        {
            var form = new UserCreateForm();
            form.FormClosed += UserCreateFormClodes;
            form.ShowDialog();
        }

        private void UserCreateFormClodes(object? sender, FormClosedEventArgs e)
        {
            LoadDefaultData();
        }

        private void AddApartmentBtn_Click(object sender, EventArgs e)
        {
            var form = new ApartmentCreateForm();
            form.FormClosed += ApartmentCreateFormClosed;
            form.ShowDialog();
        }

        private void ApartmentCreateFormClosed(object? sender, FormClosedEventArgs e)
        {
            LoadDefaultData();
        }

        private void AddResidentBtn_Click(object sender, EventArgs e)
        {
            var form = new UserCreateForm();
            form.FormClosed += UserCreateFormClodes;
            form.ShowDialog();
        }

        private void ClientInput_TextChanged(object sender, EventArgs e)
        {

        }

        private void ClientInput_KeyUp(object sender, KeyEventArgs e)
        {
            string searchText = ClientInput.Text.ToLower();
            int cursorPosition;
            if (!string.IsNullOrWhiteSpace(searchText))
            {
                using (AppDbContext db = new AppDbContext())
                {
                    // Фильтрация и сортировка пользователей
                    var filteredUsers = db.Users
                    .Where(u => u.Name.ToLower().Contains(searchText))
                    .OrderBy(u => u.Name)
                    .ToList();

                    // Сохраняем текущую позицию курсора
                    cursorPosition = ClientInput.SelectionStart;
                    if (filteredUsers.Count() > 0)
                    {
                        // Обновление элементов в выпадающем списке
                        ClientInput.Items.Clear();
                        foreach (var user in filteredUsers)
                        {
                            ClientInput.Items.Add(user);
                        }
                    }
                    else
                    {
                        MessageBox.Show("Пользователь не найден!");
                    }
                }
                // Восстанавливаем текст и позицию курсора
                ClientInput.Text = searchText;
                ClientInput.SelectionStart = cursorPosition;
            }
            else
            {
                LoadDefaultData();
            }
        }

        private void ResidentInput_KeyUp(object sender, KeyEventArgs e)
        {
            string searchText = ResidentInput.Text.ToLower();
            int cursorPosition;
            if (!string.IsNullOrWhiteSpace(searchText))
            {
                using (AppDbContext db = new AppDbContext())
                {
                    // Фильтрация и сортировка пользователей
                    var filteredUsers = db.Users
                    .Where(u => u.Name.ToLower().Contains(searchText))
                    .OrderBy(u => u.Name)
                    .ToList();

                    // Сохраняем текущую позицию курсора
                    cursorPosition = ResidentInput.SelectionStart;
                    if (filteredUsers.Count() > 0)
                    {
                        // Обновление элементов в выпадающем списке
                        ResidentInput.Items.Clear();
                        foreach (var user in filteredUsers)
                        {
                            ResidentInput.Items.Add(user);
                        }
                    }
                    else
                    {
                        MessageBox.Show("Пользователь не найден!");
                    }
                }
                // Восстанавливаем текст и позицию курсора
                ResidentInput.Text = searchText;
                ResidentInput.SelectionStart = cursorPosition;
            }
            else
            {
                LoadDefaultData();
            }
        }

        private void ApartmentInput_KeyUp(object sender, KeyEventArgs e)
        {
            string searchText = ApartmentInput.Text.ToLower();
            int cursorPosition;
            if (!string.IsNullOrWhiteSpace(searchText))
            {
                using (AppDbContext db = new AppDbContext())
                {
                    var filteredApartments = db.ApartmentBuildings
                        .Where(u => u.City.ToLower().Contains(searchText) ||
                                    u.Street.ToLower().Contains(searchText) ||
                                    u.HouseNumber.ToString().ToLower().Contains(searchText))
                        .ToList();

                    // Сохраняем текущую позицию курсора
                    cursorPosition = ApartmentInput.SelectionStart;
                    if (filteredApartments.Count() > 0)
                    {
                        // Обновление элементов в выпадающем списке
                        ApartmentInput.Items.Clear();
                        foreach (var x in filteredApartments)
                        {
                            ApartmentInput.Items.Add(
                                new ApartmentDTO
                                {
                                    Id = x.Id,
                                    Address = $"{x.City}, {x.Street}, д.{x.HouseNumber}"
                                }
                            );
                        }
                    }
                    else
                    {
                        MessageBox.Show("Адрес не найден!");
                    }
                }
                // Восстанавливаем текст и позицию курсора
                ApartmentInput.Text = searchText;
                ApartmentInput.SelectionStart = cursorPosition;
            }
            else
            {
                LoadDefaultData();
            }
        }
    }
}
