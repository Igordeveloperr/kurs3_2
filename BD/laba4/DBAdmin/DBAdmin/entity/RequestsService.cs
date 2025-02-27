using System.ComponentModel.DataAnnotations.Schema;

namespace DBAdmin.entity
{
    [Table("requests_services")]
    public class RequestsService
    {
        [Column("id")]
        public int Id { get; set; }

        [Column("service_id")]
        public int ServiceId { get; set; }
        public RequestServiceToPosition? ServiceToPosition { get; set; }

        [Column("request_id")]
        public int RequestId { get; set; }
        public RequestClientApplication? ClientApplication { get; set; }
    }
}
