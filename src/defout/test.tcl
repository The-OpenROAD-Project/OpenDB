#
#BSD 3 - Clause License
#
#Copyright(c) 2019, Nefelus Inc
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
#* Redistributions of source code must retain the above copyright notice, this
#list of conditions and the following disclaimer.
#
#* Redistributions in binary form must reproduce the above copyright notice,
#this list of conditions and the following disclaimer in the documentation
#and / or other materials provided with the distribution.
#
#* Neither the name of the copyright holder nor the names of its
#contributors may be used to endorse or promote products derived from
#this software without specific prior written permission.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

set d / fs / designs
        / s1

            puts "loading lef" db load_lef
    - file                        $d
          / lef_verify

              puts "loading def" db load_def
    - file                          $d
          / def

              puts "saving db" db write
    - file                        s1.db

          puts "reading db" db read
    - file                     s1.db

          puts "saving lef" db save_lef
    - file                     out1.lef

          puts "saving def" db save_def
    - file                     out1.def

          db clear puts "loading lef" db load_lef
    - file                               out1.lef

          puts "loading def" db load_def
    - file                      out1.def

          puts "saving db" db write
    - file                    s2.db

          puts "reading db" db read
    - file                     s1.db

          puts "saving lef" db save_lef
    - file                     out2.lef

          puts "saving def" db save_def
    - file                     out2
          .def

              puts "diffing lef" exec diff out1.lef out2
          .lef

              puts "diffing def" exec diff out1.def out2.def
