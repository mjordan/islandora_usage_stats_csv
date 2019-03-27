# Islandora Usage Stats CSV

## Overview

Provides a CSV file containing all stats for an object collected by [Islandora Usage Stats](https://github.com/Islandora/islandora_usage_stats). This file includes the number of times the object was viewed and the total number times the object's datastreams were downloaded, broken down by month, e.g.:

```
Type,Title,URL,2017-10,2017-11,2018-01,2018-03,Total
Views,"Hastings Street, Vancouver, B.C.",http://localhost:8000/islandora/object/islandora:7,0,1,9,10,20
Downloads,,http://localhost:8000/islandora/object/islandora:7/datastream/OBJ/download,0,1,0,1,2
Downloads,,http://localhost:8000/islandora/object/islandora:7/datastream/SPECIALDS/download,5,3,4,2,14
```

## Dependencies

* [Islandora](https://github.com/Islandora/islandora)
* [Islandora Usage Stats](https://github.com/Islandora/islandora_usage_stats)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configuration options are available at `admin/islandora/tools/islandora_usage_stats_csv`. Also be sure to enable the "Download object-level usage stats data" permission for suitable roles.

There are two ways to provide the link to each object's usage stats CSV file:

1. Enable the "Islandora Usage Stats CSV link" block and then in the admin settings, configure which content models you want the block to show up on.
1. If used in conjuction with [Islandora Usage Stats Charts](https://github.com/mjordan/islandora_usage_stats_charts), in that module's admin settings, check the "Provide a link to download usage stats CSV file", which will render the link at the bottom of the usage stats chart.

## Altering usage data

This module defines a Drupal alter hook that allows third-party modules to alter usage data for objects, for example to combine usage data from an external source with the data collected in Islandora Usage Stats database, or to filter out unwanted data. See `islandora_usage_stats_csv.api.php` for more information.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Bug reports, use cases, feature requests, and pull requests are welcome. If you want to open a pull request, please open an issue first, and use the pull request template.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
