using System.ComponentModel.DataAnnotations.Schema;

namespace DBAdmin.entity
{
    [Table("apartment_buildings_object_types")]
    public class ApartmentBuildingObjectType
    {
        [Column("id")]
        public int Id { get; set; }

        [Column("type")]
        public int Type { get; set; }
    }
}
