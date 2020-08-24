/* # EC-CUBE ACL CHMOD v1.0 2020-08-24
   ## (Add Unix-chmod-like Access Control Lists to EC-CUBE 4)
   ## Copyright 2020, Eido Inoue
 
   This program adds entries into the authority and role tables
   within an EC-CUBE database so that a store administrator can
   specify exactly which of the 6 major tabs each person has access to.
*/
/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// each one of these corresponds to a UI tab / route in the EC-CUBE backoffice

const int product  = 001;
const int order    = 002;
const int customer = 004;
const int content  = 010;
const int admin    = 020;
const int store    = 040;
const int bitmask = store | admin | content | customer | order | product;

const int creator_id = 1;   // account should be pre-created owner/admin

int insert_mtb_authority(int authority_id, int id) {
    char name[7] = { '\0' };
    int role = authority_id & bitmask;
    const char *fmt =
        "INSERT INTO `mtb_authority` "
        "(`id`, `name`, `sort_no`, `discriminator_type`) "
        "VALUES ('%d', '%s', '%d', '%s');\n";

    // coincidentally these are ordered consonant/vowel so easy to remember

    if (role & product)  strcat(name, "P");
    if (role & order)    strcat(name, "O");
    if (role & customer) strcat(name, "C");
    if (role & content)  strcat(name, "D"); // "D" for design rather than re-use overloaded "C"
    if (role & admin)    strcat(name, "A");
    if (role & store)    strcat(name, "S");

    if (strlen(name) == 0) strcpy(name, "------");
    
    printf(fmt, role, name, ++id, "authority");

    return id;
}

int insert_dtb_authority_role(int id, int authority_id, char *create_date) {
    const char *fmt =
        "INSERT INTO `dtb_authority_role` "
        "(`id`, `authority_id`, `creator_id`, `deny_url`, `create_date`, `update_date`, `discriminator_type`) "
        "VALUES ('%d', '%d', '%d', '%s', '%s', '%s', '%s');\n";

    int deny = ~authority_id & bitmask;

    if (deny & product)
        printf(fmt, ++id, authority_id, creator_id, "/product",  create_date, create_date, "authorityrole");
    if (deny & order)
        printf(fmt, ++id, authority_id, creator_id, "/order",    create_date, create_date, "authorityrole");
    if (deny & customer)
        printf(fmt, ++id, authority_id, creator_id, "/customer", create_date, create_date, "authorityrole");
    if (deny & content)
        printf(fmt, ++id, authority_id, creator_id, "/content",  create_date, create_date, "authorityrole");
    if (deny & admin)
        printf(fmt, ++id, authority_id, creator_id, "/setting",  create_date, create_date, "authorityrole");
    if (deny & store)
        printf(fmt, ++id, authority_id, creator_id, "/store",    create_date, create_date, "authorityrole");

    return id;
}


int main(int argc, char *argv[])
{
    time_t unixtime;
    struct tm *timestamp;
    char iso8601[20];
    const char *fmt = "%F %X";  // YYYY-MM-DD HH:MM:SS

    if (argc != 2) {
        fprintf(stderr, "Usage: %s eccube_database_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    time(&unixtime);
    timestamp = localtime(&unixtime);

    strftime(iso8601, sizeof iso8601, fmt, timestamp);

    printf("USE %s;\n", argv[1]);
    for (int i = 0, id = 0; i <= bitmask; i++)
        id = insert_mtb_authority(id, i);

    for (int i = 0, id = 0; i <= bitmask; i++)
        id = insert_dtb_authority_role(id, i, iso8601);

    return 0;
}