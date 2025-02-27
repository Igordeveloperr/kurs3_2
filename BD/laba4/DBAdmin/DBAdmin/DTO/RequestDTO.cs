using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DBAdmin.DTO
{
    internal class RequestDTO
    {
        public int Id { get; set; }
        public string ResidentName { get; set; }
        public string Comments { get; set; }
        public string Address { get; set; }
        public string ClientName { get; set; }
    }
}
