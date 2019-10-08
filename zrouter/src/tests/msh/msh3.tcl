#
# test: respawn test
#
msh::init

# -- create a remote task
spawn t "info hostname" -timeout 100


monitor "t" {
        done { puts [$task(task) Status]
            puts [$task(task) cget -serial]
        }
}
# -- create a clone of t
respawn t t1

monitor "t1" {
        done { puts [$task(task) Status]
            puts [$task(task) cget -serial]
        }
}
