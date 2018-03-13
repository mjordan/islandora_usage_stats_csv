<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Retrieve views usage data for object views from the db.
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
  // Total all values up.
  $total = 0;
  foreach ($views_data as $month => $count) {
    $total = $total + $count;
  }
  $views_data['Total'] = $total;
  return $views_data;
}

/**
 * Retrieve usage data for all datastream downloads from the db.
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
  // Total all values up.
  $total = 0;
  foreach ($downloads_data as $month => $count) {
    $total = $total + $count;
  }
  $downloads_data['Total'] = $total;
  return $downloads_data;
}

/**
 * Retrieve usage data for each downloaded datastream from the db.
 */
function islandora_usage_stats_csv_generate_downloads_per_ds_stats($pid) {
  $access_log_id = islandora_usage_stats_csv_get_object_access_id($pid);
  $object = islandora_object_load($pid);
  $datastream_ids = array();
  $datastreams_to_skip = explode(',', variable_get('islandora_usage_stats_csv_dsids_to_skip', 'RELS-EXT,RELS-INT'));
  foreach ($object as $ds) {
    if (!in_array($ds->id, $datastreams_to_skip)) {
      $datastream_ids[] = $ds->id;
    }
  }

  $downloads_data = array();
  foreach ($datastream_ids as $dsid) {
    $downloads_result = db_query("SELECT time FROM {islandora_usage_stats_datastreams}, {islandora_usage_stats_object_ds_access_log} WHERE {islandora_usage_stats_datastreams}.pid_id = :pid_id AND dsid = :dsid AND {islandora_usage_stats_object_ds_access_log}.ds_id = {islandora_usage_stats_datastreams}.id", array(':pid_id' => $access_log_id, ':dsid' => $dsid));
    foreach ($downloads_result as $row) {
      $date = date("Y-m", $row->time);
      if (!array_key_exists($dsid, $downloads_data)) {
        $downloads_data[$dsid] = array();
      }

      if (array_key_exists($date, $downloads_data[$dsid])) {
        $downloads_data[$dsid][$date]++;
      }
      else {
        $downloads_data[$dsid][$date] = 1;
      }
    }
  }

  // Total all values up.
  $tmp_downloads_data = $downloads_data;
  foreach ($tmp_downloads_data as $dsid => $months) {
    $total = 0;
    foreach ($months as $month => $count) {
      $total = $total + $count;
    }
    $downloads_data[$dsid]['Total'] = $total;
  }

  return $downloads_data;
}

/**
 * Generate the CSV file.
 *
 * @param object $object
 *   The Islandora object to generate the CSV data for.
 * @param bool $per_ds
 *   A flag indicating that the CSV data should break out
 *   stats for each downloaded datastream.
 *
 * @return string
 *   The CSV data.
 */
function islandora_usage_stats_csv_generate_csv($object, $per_ds = FALSE) {
  global $base_url;
  $views_data = islandora_usage_stats_csv_generate_views_stats($object->id);
  $csv_rows = array();

  if (variable_get('islandora_usage_stats_csv_per_ds', 1)) {
    $dsids_downloads_data = islandora_usage_stats_csv_generate_downloads_per_ds_stats($object->id);
    $downloads_months = array();
    foreach ($dsids_downloads_data as $dsid => $dates) {
      $dsid_months = array_keys($dates);
      $downloads_months = array_merge($downloads_months, $dsid_months);
    }
  }
  else {
    $downloads_data = islandora_usage_stats_csv_generate_downloads_stats($object->id);
    $downloads_months = array_keys($downloads_data);
  }

  // Assemble header row.
  $views_months = array_keys($views_data);
  $months = array_merge($views_months, $downloads_months);
  $months = array_unique($months);
  sort($months, SORT_NATURAL);
  $views_label = variable_get('islandora_usage_stats_csv_object_views_label', 'Item');
  $headers = array('Usage type', $views_label, 'URL');
  $headers = array_merge($headers, $months);
  $csv_rows[] = $headers;

  // Add views usage data.
  $views_row = array(
    variable_get('islandora_usage_stats_csv_object_views_label', 'Item'),
    $object->label,
    $base_url . '/islandora/object/' . $object->id,
  );
  foreach ($months as $month) {
    if (in_array($month, array_keys($views_data))) {
      $views_row[] = $views_data[$month];
    }
    else {
      $views_row[] = '0';
    }
  }
  $csv_rows[] = $views_row;

  // Add downloads usage data.
  if ($per_ds) {
    foreach ($dsids_downloads_data as $dsid => $monthly_data) {
      $downloads_row = array(
        'Downloads',
        '',
        $base_url . '/islandora/object/' . $object->id . '/datastream/' . $dsid . '/download',
      );
      foreach ($months as $month) {
        if (array_key_exists($month, $monthly_data)) {
          $downloads_row[] = $dsids_downloads_data[$dsid][$month];
        }
        else {
          $downloads_row[] = '0';
        }
      }
      $csv_rows[] = $downloads_row;
    }
  }
  else {
    $downloads_row = array('Downloads', '', '');
    foreach ($months as $month) {
      if (in_array($month, array_keys($downloads_data))) {
        $downloads_row[] = $downloads_data[$month];
      }
      else {
        $downloads_row[] = '0';
      }
    }
    $csv_rows[] = $downloads_row;
  }

  // Assemble CSV file and prepare it for download.
  $csv_pointer = fopen('php://temp', 'w+');
  foreach ($csv_rows as $fields) {
    fputcsv($csv_pointer, $fields);
  }
  rewind($csv_pointer);
  $csv_output = stream_get_contents($csv_pointer);
  fclose($csv_pointer);

  return $csv_output;
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
