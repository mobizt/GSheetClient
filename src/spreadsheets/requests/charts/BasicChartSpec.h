#ifndef BASIC_CHART_SPEC_H
#define BASIC_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/Theme.h"
#include "./spreadsheets/requests/CellFormat.h"

/**
 * BasicChartSpec       (enum) BasicChartType
 *                      (enum) BasicChartLegendPosition
 * 
 *                      BasicChartAxis                          (enum) BasicChartAxisPosition
 *                                                              TextFormat
 *                                                              TextPosition
 *                                                              ChartAxisViewWindowOptions              (enum) ViewWindowMode
 * 
 *                      BasicChartDomain                        ChartData*
 * 
 *                      BasicChartSeries                        ChartData*
 *                                                              (enum) BasicChartAxisPosition
 *                                                              (enum) BasicChartType
 *                                                              LineStyle                               (enum) LineDashType
 * 
 *                                                              DataLabel*
 *                                                              ColorStyle*
 *                                                              PointStyle                              (enum) PointShape
 * 
 *                                                              BasicSeriesDataPointStyleOverride       ColorStyle*
 *                                                                                                      PointStyle                      (enum) PointShape
 *                      (enum) BasicChartStackedType
 *                      (enum) BasicChartCompareMode
 *                      DataLabel*
 * 
 * See Theme.h, ChartData.h, DataLabel.h
*/
namespace GSHEET
{
}

#endif