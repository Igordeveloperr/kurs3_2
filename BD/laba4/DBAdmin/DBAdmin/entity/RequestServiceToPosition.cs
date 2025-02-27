using System.ComponentModel.DataAnnotations.Schema;

namespace DBAdmin.entity
{
    [Table("requests_services_to_position")]
    public class RequestServiceToPosition
    {
        [Column("id")]
        public int Id { get; set; }

        [Column("price")]
        public int Price { get; set; }

        [Column("service_id")]
        public int ServiceId { get; set; }
        public RequestsClientService? Service { get; set; }

        [Column("position_id")]
        public int PositionId { get; set; }
        public UserPosition? Position { get; set; }
    }
}
