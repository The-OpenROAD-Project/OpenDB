#include "rawstring_begin.txt"

set db [odb::dbDatabase_create]

namespace eval odb {
    proc read_lef args {
        return [odb_read_lef {*}$args]
    }
    proc read_def args {
        return [odb_read_def {*}$args]
    }
    proc read_design args {
        return [odb_read_design {*}$args]
    }
    proc write_lef args {
        return [odb_write_lef {*}$args]
    }
    proc write_def args {
        return [odb_write_def {*}$args]
    }
    proc import_db args {
        return [odb_import_db {*}$args]
    }
    proc export_db args {
        return [odb_export_db {*}$args]
    }
    proc get_db {} {
        global db
        return $db
    }
    namespace export *
}


)====="