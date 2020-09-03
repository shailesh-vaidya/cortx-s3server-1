#!/bin/bash -e
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
##################################
# Configure replication 
##################################

usage() { echo "Usage: [-h <provide file containing hostnames of nodes in cluster>],[-p <ldap admin password>]" 1>&2; exit 1; }

while getopts ":h:p:" o; do
    case "${o}" in
        h)
            host_list=${OPTARG}
            ;;
        p)
            password=${OPTARG}
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if [ -z ${host_list} ] || [ -z ${password} ]
then
    usage
    exit 1
fi

id=1
getServerId()
{
while read host; do
if [ "$host" == "$HOSTNAME" ]
then
  echo "equal"
  break
fi
id=`expr ${id} + 1`
done <$host_list
}

#olcServerId script
getServerId
sed -e "s/\${serverid}/$id/" serverIdTemplate.txt > scriptServerId.ldif
ldapmodify -Y EXTERNAL  -H ldapi:/// -f scriptServerId.ldif
rm scriptServerId.ldif

ldapadd -Y EXTERNAL -H ldapi:/// -f syncprov_mod.ldif

ldapadd -Y EXTERNAL -H ldapi:/// -f syncprov.ldif

#update replicaiton config

rid=1
while read host; do
sed -e "s/\${rid}/$rid/" -e "s/\${provider}/$host/" -e "s/\${credentials}/$password/" configTemplate.txt > scriptConfig.ldif
if [ ${rid} -eq 2 ] && [ ${id} -eq 1 ]
then
echo "-" >> scriptConfig.ldif
echo "add: olcMirrorMode" >> scriptConfig.ldif
echo "olcMirrorMode: TRUE" >> scriptConfig.ldif
fi
if [ ${rid} -eq 1 ] && [ ${id} -ne 1 ]
then
echo "-" >> scriptConfig.ldif
echo "add: olcMirrorMode" >> scriptConfig.ldif
echo "olcMirrorMode: TRUE" >> scriptConfig.ldif
fi
ldapmodify -Y EXTERNAL  -H ldapi:/// -f scriptConfig.ldif
rm scriptConfig.ldif
rid=`expr ${rid} + 1`
done <$host_list

iteration=1
# Update mdb file
while read host; do
sed -e "s/\${rid}/$rid/" -e "s/\${provider}/$host/" -e "s/\${credentials}/$password/" dataTemplate.txt > scriptData.ldif
if [ ${iteration} -eq 2 ] && [ ${id} -eq 1 ]
then
echo "-" >> scriptData.ldif
echo "add: olcMirrorMode" >> scriptData.ldif
echo "olcMirrorMode: TRUE" >> scriptData.ldif
fi
if [ ${iteration} -eq 1 ] && [ ${id} -ne 1 ]
then
echo "-" >> scriptData.ldif
echo "add: olcMirrorMode" >> scriptData.ldif
echo "olcMirrorMode: TRUE" >> scriptData.ldif
fi
ldapmodify -Y EXTERNAL  -H ldapi:/// -f scriptData.ldif
rm scriptData.ldif
rid=`expr ${rid} + 1`
iteration=`expr ${iteration} + 1`
done <$host_list
