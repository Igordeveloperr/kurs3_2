using DBAdmin.context;
using DBAdmin.DTO;
using DBAdmin.entity;
using Microsoft.EntityFrameworkCore;
using Microsoft.VisualBasic.ApplicationServices;
using System.Drawing.Printing;
using System.Windows.Forms;

namespace DBAdmin
{
    public partial class Form1 : Form
    {
        private const int VIEW_BTN = 0;
        private const int UPDATE_BTN = 1;
        private const int DELETE_BTN = 2;
        private const int PAGE_SIZE = 20;

        private int _currentPage = 1;

        public Form1()
        {
            InitializeComponent();
            for(int i = 0; i < RequestTable.Columns.Count; i++)
            {
                RequestTable.Columns[i].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            }
            UpdateRequestTable();
        }
        
        // обновление данных в таблице
        private void UpdateRequestTable()
        {
            using (AppDbContext db = new AppDbContext())
            {
                RequestTable.DataSource = LoadRequestsTable();

                RequestTable.Columns["Id"].Visible = false;
                RequestTable.Columns["ResidentName"].Visible = false;
                RequestTable.Columns["Comments"].Visible = false;
                RequestTable.Columns["Address"].Visible = false;
                RequestTable.Columns["ClientName"].Visible = false;

                RequestTable.Columns["ResidentName1"].DataPropertyName = "ResidentName";
                RequestTable.Columns["Comments1"].DataPropertyName = "Comments";
                RequestTable.Columns["Address1"].DataPropertyName = "Address";
                RequestTable.Columns["ClientName1"].DataPropertyName = "ClientName";

                RequestTable.Refresh();
            }
            UpdateNavigationButtons();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        // загрузка заявок с пагинацией
        private List<RequestDTO> LoadRequestsTable()
        {
            List<RequestDTO> data = new List<RequestDTO>();
            using (AppDbContext db = new AppDbContext())
            {
                var requests = db.RequestClientApplications
                    .Include(x => x.Apartment)
                    .Include(x => x.Client)
                    .OrderByDescending(x => x.Id) // Замените YourProperty на свойство, по которому хотите сортировать
                    .Skip((_currentPage - 1) * PAGE_SIZE)
                    .Take(PAGE_SIZE)
                    .ToList();

                foreach (var request in requests)
                {
                    data.Add(new RequestDTO
                    {
                        Id = request.Id,
                        ResidentName = request.ResidentName,
                        Comments = request.Comments,
                        Address = $"{request.Apartment.City}, {request.Apartment.Street}, д.{request.Apartment.HouseNumber}",
                        ClientName = request.Client.Name
                    });
                }
            }
            return data.OrderByDescending(x => x.Id).ToList();
        }

        // удаление заявки
        private void DeleteRequestApplication(int id)
        {
            var warningForm = new ConfirmDeletele(id);
            warningForm.OnClose += ConfirmDeleteleClosed;
            warningForm.ShowDialog();
        }

        private void ConfirmDeleteleClosed(bool flag, int id)
        {
            if (flag)
            {
                using (AppDbContext db = new AppDbContext())
                {
                    var application = db.RequestClientApplications.Where(x => x.Id == id).FirstOrDefault();
                    if (application != null)
                    {
                        db.RequestClientApplications.Remove(application);
                        db.SaveChanges();
                    }
                }
                UpdateRequestTable();
            }
        }

        // обновить заявку
        private void ViewBtnClick(int id)
        {
            var form = new RequestViewForm(id);
            form.ShowDialog();
        }

        // обновить заявку
        private void UpdateBtnClick(int id)
        {
            var form = new RequestUpdateForm(id);
            form.FormClosed += RequestUpdateFormClosed;
            form.ShowDialog();
        }

        private void RequestUpdateFormClosed(object? sender, FormClosedEventArgs e)
        {
            UpdateRequestTable();
        }

        // обработка кнопок в таблице
        private void RequestTable_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            if (((DataGridView)sender).Columns[e.ColumnIndex] is DataGridViewButtonColumn && e.RowIndex >= 0)
            {
                int id = (int)RequestTable.Rows[e.RowIndex].Cells["Id"].Value;
                switch (e.ColumnIndex)
                {
                    case VIEW_BTN:
                        ViewBtnClick(id);
                        break;
                    case UPDATE_BTN:
                        UpdateBtnClick(id);
                        break;
                    case DELETE_BTN:
                        DeleteRequestApplication(id);
                        break;
                }
            }
        }

        // обновить таблицу
        private void UpdateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            UpdateRequestTable();
        }

        // создать заявку
        private void CreateRequestToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var form = new CreateRequestForm();
            form.FormClosed += CreateRequestFormClised;
            form.ShowDialog();
        }

        // обновляем таблицу при закрытии формы создания заявки
        private void CreateRequestFormClised(object? sender, FormClosedEventArgs e)
        {
            UpdateRequestTable();
        }

        // страницы назад
        private void prevBtn_Click(object sender, EventArgs e)
        {
            if (_currentPage > 1)
            {
                _currentPage--;
                UpdateRequestTable();
            }
        }

        // страницы вперед
        private void nextBtn_Click(object sender, EventArgs e)
        {
            using(AppDbContext db = new AppDbContext())
            {
                if (_currentPage * PAGE_SIZE < db.RequestClientApplications.Count())
                {
                    _currentPage++;
                    UpdateRequestTable();
                }
            }
        }

        // блокировка и разблокировка кнопок пагинации
        private void UpdateNavigationButtons()
        {
            prevBtn.Enabled = _currentPage > 1;
            using (AppDbContext db = new AppDbContext())
            {
                nextBtn.Enabled = _currentPage * PAGE_SIZE < db.RequestClientApplications.Count();
            }
        }
    }
}
