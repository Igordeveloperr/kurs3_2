using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DBAdmin.DTO
{
    public class ErrorDTO
    {
        public bool Result { get; set; }
        public string Message { get; set; }

        public ErrorDTO()
        {
            Result = false;
            Message = "";
        }
    }
}
