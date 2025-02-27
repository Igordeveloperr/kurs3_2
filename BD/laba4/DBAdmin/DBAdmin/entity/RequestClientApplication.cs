using System.ComponentModel.DataAnnotations.Schema;

namespace DBAdmin.entity
{
    [Table("requests_client_applications")]
    public class RequestClientApplication
    {
        [Column("id")]
        public int Id { get; set; }

        [Column("client_id")]
        public int ClientId { get; set; }
        public User? Client { get; set; }

        [Column("object_type_id")]
        public int ObjectTypeId { get; set; }
        public ApartmentBuildingObjectType? ObjectType { get; set; }

        [Column("object_num_id")]
        public int ObjectNumId { get; set; }
        public ApartmentBuilding? Apartment { get; set; }

        [Column("comments")]
        public string Comments { get; set; }

        [Column("resident_name")]
        public string ResidentName { get; set; }
    }
}
