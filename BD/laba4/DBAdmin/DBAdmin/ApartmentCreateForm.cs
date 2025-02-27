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
    public partial class ApartmentCreateForm : Form
    {
        private const int MIN_CITY_LEN = 3;
        private const int MIN_STREET_LEN = 4;
        private const int MIN_HOUSE_LEN = 1;
        public ApartmentCreateForm()
        {
            InitializeComponent();
        }

        private void ApartmentCreateForm_Load(object sender, EventArgs e)
        {

        }

        private void CreateApartmentBtn_Click(object sender, EventArgs e)
        {
            ErrorDTO err1 = ValidateCity();
            ErrorDTO err2 = ValidateStreet();
            ErrorDTO err3 = ValidateHouse();

            if (!err1.Result && !err2.Result && !err3.Result)
            {
                AddApartmentToDb();
            }
            else if (err1.Result)
            {
                MessageBox.Show(err1.Message);
            }
            else if (err2.Result)
            {
                MessageBox.Show(err2.Message);
            }
            else if (err3.Result)
            {
                MessageBox.Show(err3.Message);
            }
        }

        private void AddApartmentToDb()
        {
            using (AppDbContext db = new AppDbContext())
            {
                var exApartment = db.ApartmentBuildings
                    .Where(x =>
                    x.City == CityInput.Text
                    && x.Street == StreetInput.Text
                    && x.HouseNumber == Convert.ToInt32(HouseInput.Text)
                    ).ToList().FirstOrDefault();

                if (exApartment == null)
                {
                    db.ApartmentBuildings.Add(new entity.ApartmentBuilding
                    {
                        City = CityInput.Text,
                        Street = StreetInput.Text,
                        HouseNumber = Convert.ToInt32(HouseInput.Text)
                    });
                    db.SaveChanges();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Данный адрес уже существует!");
                }
            }
            
        }

        // проверка города
        private ErrorDTO ValidateCity()
        {
            ErrorDTO dto = new ErrorDTO();
            if (CityInput.Text.Length < MIN_CITY_LEN)
            {
                dto.Result = true;
                dto.Message = $"Минимальная длина города {MIN_CITY_LEN} символов!";
            }
            else if (string.IsNullOrWhiteSpace(CityInput.Text))
            {
                dto.Result = true;
                dto.Message = "Город не может быть пустым!";
            }
            return dto;
        }

        // проверка улицы
        private ErrorDTO ValidateStreet()
        {
            ErrorDTO dto = new ErrorDTO();
            if (StreetInput.Text.Length < MIN_STREET_LEN)
            {
                dto.Result = true;
                dto.Message = $"Минимальная длина улицы {MIN_STREET_LEN} символов!";
            }
            else if (string.IsNullOrWhiteSpace(StreetInput.Text))
            {
                dto.Result = true;
                dto.Message = "Улица не может быть пустым!";
            }
            return dto;
        }

        // проверка дома
        private ErrorDTO ValidateHouse()
        {
            ErrorDTO dto = new ErrorDTO();
            if (HouseInput.Text.Length < MIN_HOUSE_LEN)
            {
                dto.Result = true;
                dto.Message = $"Минимальная длина дома {MIN_HOUSE_LEN} символов!";
            }
            else if (string.IsNullOrWhiteSpace(HouseInput.Text))
            {
                dto.Result = true;
                dto.Message = "Дом не может быть пустым!";
            }
            return dto;
        }

        private void HouseInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            char number = e.KeyChar;
            if (!Char.IsDigit(number) && number != 8) // цифры, клавиша BackSpace
            {
                e.Handled = true;
            }
        }
    }
}
