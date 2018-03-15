<?php

/**
 * @file
 * Documents the hooks this module defines.
 */

/**
 * Allows modules to alter the array containing object usage entries.
 *
 * Useful for integrating usage stats not managed by Islandora Usage Stats.
 * Could also be used to filter out unwanted entries.
 *
 * @param array $usage
 *   The array of usage entries managed by Islandora Usage Stats. Entries
 *   for 'views' and 'downloads' looks like this:
 *     Array
 *      (
 *         [2017-12] => 10
 *         [2018-03] => 25
 *      )
 *    The keys are months in yyyy-mm format and the values are the number of
 *    hits in that month.
 *
 *    Usage data for datastream-specific downloads have keys that correspond
 *    to DSIDs, and within each DSID array member, the same date => count
 *    usage data as 'views' and 'downloads'. This is illustrated in the example
 *    below.
 * @param arrary $context
 *   Contains two keys, 'pid' and 'type'. 'pid' is the PID of the object,
 *   and 'type' is one of 'views', 'downloads', or 'downloads_per_ds'.
 */
function mymodule_islandora_usage_stats_csv_usage_alter(&$usage, &$context) {
  // Add a views data point.
  if ($context['type'] == 'views' and $context['pid'] == 'islandora:100') {
    $usage['1999-12'] = 100;
  }
  // Add a downloads data point.
  if ($context['type'] == 'downloads' and $context['pid'] == 'islandora:100') {
    $usage['1999-12'] = 100;
  }
  // Add a datastream-specific downloads data point.
  if ($context['type'] == 'downloads_per_ds' and $context['pid'] == 'islandora:100') {
    if (array_key_exists('OBJ', $usage)) {
      $usage['OBJ']['1999-12'] = 100;
    }
  }
}
