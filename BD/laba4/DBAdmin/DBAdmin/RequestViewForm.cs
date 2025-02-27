using DBAdmin.context;
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

namespace DBAdmin
{
    public partial class RequestViewForm : Form
    {
        private int _requestId;

        public RequestViewForm(int id)
        {
            InitializeComponent();
            _requestId = id;
            SetDefaultValues(id);
        }

        private void SetDefaultValues(int id)
        {
            using (AppDbContext db = new AppDbContext())
            {
                var application = db.RequestClientApplications
                    .Where(x => x.Id == id)
                    .Include(x => x.Client)
                    .Include(x => x.Apartment)
                    .FirstOrDefault();

                if (application != null)
                {
                    ClientInput.Text = application.Client.Name;
                    AddressInput.Text = $"{application.Apartment.City}, {application.Apartment.Street}, д.{application.Apartment.HouseNumber}";
                    ResidentInput.Text = application.ResidentName;
                    Comments.Text = application.Comments;
                }
            }
        }

        private void RequestViewForm_Load(object sender, EventArgs e)
        {

        }
    }
}
