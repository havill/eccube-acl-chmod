# eccube-acl-chmod
## Adds ACLs to EC-CUBE 4.x, each group of allowed access represented by a bit in an octet like Unix.

This program adds entries into the authority and role tables within an EC-CUBE database so that a store administrator can specify exactly which of the 6 major tabs each person has access to.

| SECTION NAME | URL PREFIX | CHMOD | MNENOMIC | BINARY |
| :----:    | :---- | :----: | :---- | ----: |
| 商品管理 | /product | P----- | (P)RODUCTS | 000001 |
| 受注管理 | /order | -O---- | (O)RDERS | 000010 |
| 会員管理 | /customer | --C--- | (C)USTOMERS | 000100 |
| コンテンツ管理 | /content | ---D-- | (D)ESIGN | 001000 |
| 設定 | /setting | ----A- | (A)DMIN | 010000 |
| オーナーズストア | /store | -----S | (S)TORE | 100000 |

### Example

A EC-CUBE member listed in the メンバー管理 section of the settings (設定) that has a 権限 (permission) setting of `DAS` will be be allowed to access:

- **D |** Design (コンテンツ管理): https://site4.ec-cube.net/admin/content
- **A |** Admin (設定): https://site4.ec-cube.net/admin/setting
- **S |** Store (オーナーズストア): https://site4.ec-cube.net/store/

They will be explicitly denied access to the remaining three tabs (directories and their subdirectories / folders) in EC-CUBE 4.x:

- **P |** Product (商品管理)：*DENIED* (403)
- **O |** Orders (受注管理): *DENIED* (403)
- **C |** Customers (会員管理): *DENIED* (403)

The 権限 (permission) of `------` will deny access to all six (6) admin areas of EC-CUBE 4.

## WARNING: EC-CUBE Security Model

EC-CUBE operates with a "Allow All, Deny individually" security model. This means that if you are a EC-CUBE 4.x "member" you can access anything in the admin area unless you've been explicitly denied.

Thus, if future root / parent / main directory / folders are added to EC-CUBE's admin backoffice, either via future releases or via plugins,
members will be able to access these areas by default.

This is not the best security model, but considering that EC-CUBE has no security model setup out-of-the-box, it attempts to make lemonade from lemons.

## Installation

0. Make the C program into a binary by executing `make`.
1. Run the program, giving it one argument: the name of the EC-CUBE 4.x database in the MySQL / PostgreSQL / SQLite database that is being used.
2. The program will generate SQL (that is compatible with MySQL and possibly other databases) to standard output. Save this output.
3. Using either the MySQL client or another tool (like phpMyAdmin), execute the generated URL.
    - The SQL is intended to be run against a clean database, but it can be run against an EC-CUBE instance that already has authority entries and roles. It will not delete any existing entries. However, because it uses low numbered ordinals for the unique key ID, most likely some entries will conflict and fail to add if entries already exist. You can edit the IDs to different numbers; the ACL system is not dependent on the value of the numeric ID keys.
3. After adding the entries, you will need to delete the cache via the `キャッシュ削除` button in コンテンツ管理 under キャッシュ管理 to reflect the changes to the admin web interface.
4. In the メンバー会員 section, you will need to edit each member and change their 権限 (permission) to one of the 64 new added combinations.