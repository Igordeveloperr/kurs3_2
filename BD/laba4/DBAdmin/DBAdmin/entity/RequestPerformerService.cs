using System.ComponentModel.DataAnnotations.Schema;

namespace DBAdmin.entity
{
    [Table("requests_performers_service")]
    public class RequestPerformerService
    {
        [Column("id")]
        public int Id { get; set; }

        [Column("request_id")]
        public int RequestId { get; set; }
        public RequestClientApplication? ClientApplication { get; set; }

        [Column("service_on_request_id")]
        public int ServiceOnRequestId { get; set; }
        public RequestsService? RequestService { get; set; }

        [Column("implementer_id")]
        public int ImplementerId { get; set; }
        public User? Implementer { get; set; }
    }
}
