using System.ComponentModel.DataAnnotations.Schema;

namespace DBAdmin.entity
{
    [Table("requests_client_services")]
    public class RequestsClientService
    {
        [Column("id")]
        public int Id { get; set; }

        [Column("name")]
        public string Name { get; set; }

        [Column("description")]
        public string Description { get; set; }
    }
}
