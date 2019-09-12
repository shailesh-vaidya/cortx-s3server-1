/*
 * COPYRIGHT 2016 SEAGATE LLC
 *
 * THIS DRAWING/DOCUMENT, ITS SPECIFICATIONS, AND THE DATA CONTAINED
 * HEREIN, ARE THE EXCLUSIVE PROPERTY OF SEAGATE TECHNOLOGY
 * LIMITED, ISSUED IN STRICT CONFIDENCE AND SHALL NOT, WITHOUT
 * THE PRIOR WRITTEN PERMISSION OF SEAGATE TECHNOLOGY LIMITED,
 * BE REPRODUCED, COPIED, OR DISCLOSED TO A THIRD PARTY, OR
 * USED FOR ANY PURPOSE WHATSOEVER, OR STORED IN A RETRIEVAL SYSTEM
 * EXCEPT AS ALLOWED BY THE TERMS OF SEAGATE LICENSES AND AGREEMENTS.
 *
 * YOU SHOULD HAVE RECEIVED A COPY OF SEAGATE'S LICENSE ALONG WITH
 * THIS RELEASE. IF NOT PLEASE CONTACT A SEAGATE REPRESENTATIVE
 * http://www.seagate.com/contact
 *
 * Original author:  Rajesh Nambiar   <rajesh.nambiar@seagate.com>
 * Original author:  Kaustubh Deorukhkar   <kaustubh.deorukhkar@seagate.com>
 * Original creation date: 6-Jan-2016
 */

#pragma once

#ifndef __S3_SERVER_S3_ABORT_MULTIPART_ACTION_H__
#define __S3_SERVER_S3_ABORT_MULTIPART_ACTION_H__

#include <memory>

#include "s3_bucket_action_base.h"
#include "s3_clovis_writer.h"

class S3AbortMultipartAction : public S3BucketAction {
  std::shared_ptr<S3ObjectMetadata> object_multipart_metadata;
  std::shared_ptr<S3PartMetadata> part_metadata;
  std::shared_ptr<S3ClovisKVSReader> clovis_kv_reader;
  std::shared_ptr<S3ClovisWriter> clovis_writer;
  std::shared_ptr<S3ClovisKVSWriter> clovis_kv_writer;
  std::shared_ptr<ClovisAPI> s3_clovis_api;
  std::string upload_id;
  std::string bucket_name;
  std::string object_name;
  m0_uint128 multipart_oid;
  m0_uint128 part_index_oid;

  std::map<std::string, std::string> probable_oid_list;

  std::shared_ptr<S3ObjectMultipartMetadataFactory> object_mp_metadata_factory;
  std::shared_ptr<S3PartMetadataFactory> part_metadata_factory;
  std::shared_ptr<S3ClovisWriterFactory> clovis_writer_factory;
  std::shared_ptr<S3ClovisKVSReaderFactory> clovis_kvs_reader_factory;
  std::shared_ptr<S3ClovisKVSWriterFactory> clovis_kv_writer_factory;

 public:
  S3AbortMultipartAction(
      std::shared_ptr<S3RequestObject> req,
      std::shared_ptr<ClovisAPI> s3_clovis_api = nullptr,
      std::shared_ptr<S3BucketMetadataFactory> bucket_meta_factory = nullptr,
      std::shared_ptr<S3ObjectMultipartMetadataFactory> object_mp_meta_factory =
          nullptr,
      std::shared_ptr<S3PartMetadataFactory> part_meta_factory = nullptr,
      std::shared_ptr<S3ClovisWriterFactory> clovis_s3_writer_factory = nullptr,
      std::shared_ptr<S3ClovisKVSReaderFactory> clovis_s3_kvs_reader_factory =
          nullptr,
      std::shared_ptr<S3ClovisKVSWriterFactory> kv_writer_factory = nullptr);

  void setup_steps();
  void fetch_bucket_info_failed();
  void get_multipart_metadata();
  void get_multipart_metadata_status();
  void delete_part_index_with_parts();
  void delete_part_index_with_parts_failed();
  void delete_multipart_metadata();
  void delete_multipart_metadata_failed();
  void send_response_to_s3_client();

  void add_object_oid_to_probable_dead_oid_list();
  void add_object_oid_to_probable_dead_oid_list_failed();

  void cleanup();
  void cleanup_successful();
  void cleanup_failed();
  void cleanup_oid_from_probable_dead_oid_list();

  // Google tests
  FRIEND_TEST(S3AbortMultipartActionTest, ConstructorTest);
  FRIEND_TEST(S3AbortMultipartActionTest, GetMultiPartMetadataTest1);
  FRIEND_TEST(S3AbortMultipartActionTest, GetMultiPartMetadataTest2);
  FRIEND_TEST(S3AbortMultipartActionTest, GetMultiPartMetadataTest3);
  FRIEND_TEST(S3AbortMultipartActionTest, GetMultiPartMetadataTest5);
  FRIEND_TEST(S3AbortMultipartActionTest, GetMultiPartMetadataTest6);
  FRIEND_TEST(S3AbortMultipartActionTest, DeleteMultipartMetadataTest1);
  FRIEND_TEST(S3AbortMultipartActionTest, DeleteMultipartMetadataTest2);
  FRIEND_TEST(S3AbortMultipartActionTest, DeleteMultipartMetadataTest3);
  FRIEND_TEST(S3AbortMultipartActionTest, DeleteMultipartMetadataTest4);
  FRIEND_TEST(S3AbortMultipartActionTest, DeleteMultipartMetadataTest5);
  FRIEND_TEST(S3AbortMultipartActionTest, DeleteMultipartMetadataFailedTest);
  FRIEND_TEST(S3AbortMultipartActionTest,
              DeleteMultipartMetadataFailedToLaunchTest);
  FRIEND_TEST(S3AbortMultipartActionTest, DeletePartIndexWithPartsTest1);
  FRIEND_TEST(S3AbortMultipartActionTest, DeletePartIndexWithPartsTest2);
  FRIEND_TEST(S3AbortMultipartActionTest, DeletePartIndexWithPartsFailed);
  FRIEND_TEST(S3AbortMultipartActionTest,
              DeletePartIndexWithPartsFailedToLaunch);
  FRIEND_TEST(S3AbortMultipartActionTest, Send200SuccessToS3Client);
  FRIEND_TEST(S3AbortMultipartActionTest, Send503InternalErrorToS3Client);
  FRIEND_TEST(S3AbortMultipartActionTest, CleanupOnMetadataFailedToRemoveTest1);
  FRIEND_TEST(S3AbortMultipartActionTest, CleanupOnMetadataFailedToRemoveTest2);
  FRIEND_TEST(S3AbortMultipartActionTest, CleanupOnMetadataRemoveTest1);
  FRIEND_TEST(S3AbortMultipartActionTest, CleanupOnMetadataRemoveTest2);
};

#endif
