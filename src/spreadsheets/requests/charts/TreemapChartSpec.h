#ifndef TREEMAP_CHART_SPEC_H
#define TREEMAP_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/Theme.h"
#include "./spreadsheets/requests/CellFormat.h"

/**
 * TREEMAP CHART CLASS DEPENDENCIES
 * 
 * TreemapChartSpec         ChartData*
 *                          TextFormat
 *                          ColorStyle*
 *                          TreemapChartColorScale      ColorStyle*
 * 
 * See ChartData.h
*/
namespace GSHEET
{
}

#endif