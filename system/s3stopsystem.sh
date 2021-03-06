#!/bin/sh -e
#
# Copyright (c) 2020 Seagate Technology LLC and/or its Affiliates
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# For any questions about this software or licensing,
# please email opensource@seagate.com or cortx-questions@seagate.com.
#

# S3 server stop script in deployment environment.
#   Usage: s3stopsystem.sh <process FID>
#             where process FID: S3 server process FID generated by hare.

if [ $# -ne 1 ]; then
  echo "Invalid number of arguments passed to the script"
  echo "Usage: s3stopsystem.sh <process FID>"
  exit 1
fi

pidfile='/var/run/s3server.'$1'.pid'

if [[ -r $pidfile ]]; then
  pidstr=`cat $pidfile`
  if [ "$pidstr" != "" ]; then
    kill -s TERM $pidstr
    if [ "$?" != "0" ]; then
      rm $pidfile
    fi
  fi
fi
