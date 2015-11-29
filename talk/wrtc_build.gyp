#
# libjingle
# Copyright 2012, Google Inc.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#  3. The name of the author may not be used to endorse or promote products
#     derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

{
  'includes': [
    'build/common.gypi',
  ],
  'conditions': [
    ['OS=="linux" or OS=="win" or OS=="mac"', {
      'targets': [
        {
          'target_name': 'wrtc_build',
          'type': 'executable',
          'sources': [
            'examples/wrtc_build.cc',
          ],
          'dependencies': [
            '<(DEPTH)/third_party/jsoncpp/jsoncpp.gyp:jsoncpp',
            'libjingle.gyp:libjingle_peerconnection',
            '<@(libjingle_tests_additional_deps)',
          ],
          'conditions': [
            # TODO(ronghuawu): Move these files to a win/ directory then they
            # can be excluded automatically.
          ],  # conditions
        }, # target wrtc_build
        {
          'target_name': 'webrtc_cmd',
          'type': 'executable',
          'sources': [
            'examples/peerconnection/webrtc_cmd.cc',
          ],
          'dependencies': [
            '<(DEPTH)/third_party/jsoncpp/jsoncpp.gyp:jsoncpp',
            'libjingle.gyp:libjingle_peerconnection',
            '<@(libjingle_tests_additional_deps)',
          ],
          # TODO(ronghuawu): crbug.com/167187 fix size_t to int truncations.
          'msvs_disabled_warnings': [ 4309, ],
        }, # target webrtc_cmd
        {
          'target_name': 'datachannel',
          'type': 'executable',
          'sources': [
            'examples/peerconnection/datachannel.cc',
          ],
          'dependencies': [
            '<(DEPTH)/third_party/jsoncpp/jsoncpp.gyp:jsoncpp',
            'libjingle.gyp:libjingle_peerconnection',
            '<@(libjingle_tests_additional_deps)',
          ],
        }, # target datachannel

      ], # targets
    }],  # OS=="linux" or OS=="win" or OS="mac"
  ],
}
