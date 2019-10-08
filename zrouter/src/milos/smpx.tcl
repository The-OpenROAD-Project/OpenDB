#
# BSD 3-Clause License
#
# Copyright (c) 2019, Nefelus Inc
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## smpx.tcl
## Multiplexing module for Milos
## Author: Mattias Hembruch

package require Itcl

package provide ade::milos::smpx 0.1

itcl::class SMPXClient {

    inherit LSMPXClient;

    constructor { {address 7679@localhost} } {
        LSMPXClient::constructor -address $address
    } {
        #puts "SMPXClient constructor: $args"
        #puts "Before: [$this configure]"
        #eval configure $args
        puts "After: [$this configure]"
    }

    public method register { type addr } {
        _init
        if [catch {d Register $type $addr} rc] {
            error "Error registering plugin type $type at address $addr: $rc"
        }
        notice "Successfully registered plugin type $type at address $addr!"
    }

    public method reserve { type } {
        _init
        d Reserve $type
        notice "Reserving type $type"
        Wait S; 
        notice "Got token for type $type"
        parray S
        notice "Returning: $S(address)"
        return $S(address)
    }

    public method release { type addr } {
        d UnRegister $type $addr
    }

    protected method _init { } {
    }
    protected method d { args } {
        debug SMPX:A "Command: $args"
        set rc [uplevel $args]
        debug SMPX:A "Return: $rc"
        set rc
    }
}
