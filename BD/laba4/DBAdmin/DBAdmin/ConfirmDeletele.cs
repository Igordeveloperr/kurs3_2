using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ToolTip;

namespace DBAdmin
{
    public partial class ConfirmDeletele : Form
    {
        public event Action<bool,int> OnClose;
        private int _id;

        public ConfirmDeletele(int id)
        {
            InitializeComponent();
            _id = id;
        }

        private void yesBtn_Click(object sender, EventArgs e)
        {
            OnClose.Invoke(true, _id);
            this.Close();
        }

        private void noBtn_Click(object sender, EventArgs e)
        {
            OnClose.Invoke(false, _id);
            this.Close();
        }
    }
}
