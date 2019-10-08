#
# The session defines the operating environment for the user actions
# during the lifetime of zroute
#
# zroute always instantiates a zsession object (the only one per session)
# which in turns instantiates some standard TM objects, based on the
# selected profile
# 
# the user at any time, can change profile
#

# those are not implemented yet ...
if 0 {

# -- switch to a new profile
zsession SetProfile developer1

# -- list available profiles
zsession ListProfiles

# -- display the list of available tools (aka Techology modules)
zsession Tools

}
