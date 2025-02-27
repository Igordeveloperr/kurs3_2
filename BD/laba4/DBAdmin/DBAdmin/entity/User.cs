using System.ComponentModel.DataAnnotations.Schema;

namespace DBAdmin.entity
{
    [Table("users")]
    public class User
    {
        [Column("id")]
        public int Id { get; set; }

        [Column("name")]
        public string Name { get; set; }

        [Column("password")]
        public string Password { get; set; }
    }
}
