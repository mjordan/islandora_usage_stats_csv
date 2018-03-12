<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Get views usage data for object views.
 */
function islandora_usage_stats_csv_generate_views_stats($pid) {
  $access_log_id = islandora_usage_stats_csv_get_object_access_id($pid);
  $views_result = db_select('islandora_usage_stats_object_access_log')
    ->fields('islandora_usage_stats_object_access_log', array('time'))
    ->condition('pid_id', $access_log_id)
    ->execute();
  $views_data = array();
  foreach ($views_result as $row) {
    $date = date("Y-m", $row->time);
    if (array_key_exists($date, $views_data)) {
      $views_data[$date]++;
    }
    else {
      $views_data[$date] = 1;
    }
  }
  return $views_data;
}

/**
 * Get usage data for all datastream downloads.
 */
function islandora_usage_stats_csv_generate_downloads_stats($pid) {
  $access_log_id = islandora_usage_stats_csv_get_object_access_id($pid);
  $downloads_result = db_query("SELECT time FROM {islandora_usage_stats_datastreams}, {islandora_usage_stats_object_ds_access_log} WHERE {islandora_usage_stats_datastreams}.pid_id = :pid_id", array(':pid_id' => $access_log_id));
  $downloads_data = array();
  foreach ($downloads_result as $row) {
    $date = date("Y-m", $row->time);
    if (array_key_exists($date, $downloads_data)) {
      $downloads_data[$date]++;
    }
    else {
      $downloads_data[$date] = 1;
    }
  }
  dd($downloads_data, 'From combined');
  return $downloads_data;
}

/**
 * Get usage data for each downloaded datastream.
 */
function islandora_usage_stats_csv_generate_downloads_per_ds_stats($pid) {
  $access_log_id = islandora_usage_stats_csv_get_object_access_id($pid);
  $downloads_result = db_query("SELECT dsid, time FROM {islandora_usage_stats_datastreams}, {islandora_usage_stats_object_ds_access_log} WHERE {islandora_usage_stats_datastreams}.pid_id = :pid_id", array(':pid_id' => $access_log_id));
  $downloads_data = array();

  foreach ($downloads_result as $row) {
    $date = date("Y-m", $row->time);
    $dsid = $row->dsid;
    if (!array_key_exists($dsid, $downloads_data)) {
      $downloads_data[$dsid] = array();
    }

    if (!array_key_exists($date, $downloads_data[$dsid])) {
      $downloads_data[$dsid][$date] = 1;
    }
    else {
      $downloads_data[$dsid][$date]++;
    }

  }

  return $downloads_data;
}

/**
 * Get the ID in the object access table corresponding to the incoming PID.
 *
 * @param string $pid
 *   The object's PID.
 *
 * @return string
 *   The corresponding ID from the islandora_usage_stats_objects table.
 */
function islandora_usage_stats_csv_get_object_access_id($pid) {
  $result = db_select('islandora_usage_stats_objects')
    ->fields('islandora_usage_stats_objects', array('id'))
    ->condition('pid', $pid)
    ->execute();
  return $result->fetchAssoc();
}
