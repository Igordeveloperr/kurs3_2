using System.ComponentModel.DataAnnotations.Schema;

namespace DBAdmin.entity
{
    [Table("apartment_buildings")]
    public class ApartmentBuilding
    {
        [Column("id")]
        public int Id { get; set; }

        [Column("city")]
        public string City { get; set; }

        [Column("street")]
        public string Street { get; set; }

        [Column("house_number")]
        public int HouseNumber { get; set; }
    }
}
