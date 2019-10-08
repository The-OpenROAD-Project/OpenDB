#
# This script tests the DB tech module
# using the s1 test chip
#

db_schema -v 1

set chip s1
set d /fs/designs

# DB db is allocated already in the default profile

if ![file exists s1.db] {
	puts "loading in $chip from lef/def"
	db load_lef -file $d/$chip/lef_verify
	db load_def -file $d/$chip/def
	db write -file s1.db
} else {
	puts "loading in $chip from db dump"
	db read -file s1.db
}

#
# here is the proposed api for streaming:
#
puts "streaming the block out ..."
set fp [open "test.db" w]
db get -top -result A
set block $A(object)
db write -channel $fp -block $block /
close $fp

puts "streaming the block in ..."
set fp [open "test.db" r]
db read -channel $fp -block $block /
close $fp

#  -- to be implemented  -----
if 0 {

#
# Here is the proposed API for the object browser
#
# query the type of an object (zdbobject)
db type -object "b123"  -r A    ;# return the type info for b123
parray A
	A(type)   BLOCK
	A(chip)   CHIP
	A(tiles)  {LIST TILES}
	...

# query the type of an type
db type -type BLOCK
parray A
	A(type)   BLOCK
	A(chip)   CHIP
	A(tiles)  {LIST TILES}
	...

# query the object slots
db instance -obj b123
parray A
	A(sefl)   b123
	A(chip)   chip0
	A(tiles)  {b123.tile1 b123.tile2 ...}
	...

# query the type of an object (zobject or TM object)

db type -obj gmap12
parray A
	A(chip)   CHIL
	A(cost)   TREE
    ...

# query the contents of an object (zobject or TM object)
db instance -obj gmap12
parray A
	A(chip)   chip0
	A(cost) {
layer1 {
	{0 0 0 0 10 10 10 }
	{0 0 5 0 10 10 10 }
	{0 5 0 0 10 15 10 }
	{0 0 5 0 10 10 15 }
}
layer2 {
	{0 0 0 0 10 10 10 }
	{0 0 5 0 10 10 10 }
	{0 5 0 0 10 15 10 }
	{0 0 5 0 10 10 15 }
}
...
}
	...

}
