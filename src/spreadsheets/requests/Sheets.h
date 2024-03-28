#ifndef SHEETS_H
#define SHEETS_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/DataLabel.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/charts/BasicChartSpec.h"
#include "./spreadsheets/requests/charts/BubbleChartSpec.h"
#include "./spreadsheets/requests/charts/CandlestickChartSpec.h"
#include "./spreadsheets/requests/charts/HistogramChartSpec.h"
#include "./spreadsheets/requests/charts/OrgChartSpec.h"
#include "./spreadsheets/requests/charts/PieChartSpec.h"
#include "./spreadsheets/requests/charts/ScorecardChartSpec.h"
#include "./spreadsheets/requests/charts/TreemapChartSpec.h"
#include "./spreadsheets/requests/charts/WaterfallChartSpec.h"

/**
 * Sheet            SheetProperties         (enum) SheetType
 *                                          GridProperties
 *                                          ColorStyle*
 *                                          DataSourceSheetProperties               DataSourceColumn                    DataSourceColumnReference
 * 
 *                                                                                  DataExecutionStatus                 (enum) DataExecutionState
                                                                                                                        (enum) DataExecutionErrorCode
	
                    GridData                RowData                                 CellData*

					                        DimensionProperties                     DeveloperMetadata                   DeveloperMetadataLocation           (enum) DeveloperMetadataLocationType
                                                                                                                                                            DimensionRange

                                                                                                                        (enum) DeveloperMetadataVisibility

                                                                                    DataSourceColumnReference

                    GridRange
                    ConditionalFormatRule   GridRange
                                            BooleanRule                             BooleanCondition*
                                                                                    CellFormat*

                                            GradientRule                            InterpolationPoint                  ColorStyle
                                                                                                                        (enum) InterpolationPointType

                    FilterView              GridRange
                                            SortSpec*
                                            FilterSpec*

                    ProtectedRange          GridRange
                                            Editors

                    BasicFilter             GridRange
                                            SortSpec*
                                            FilterSpec*

                    EmbeddedChart           ChartSpec                               TextFormat
                                                                                    TextPosition
                                                                                    ColorStyle*
                                                                                    DataSourceChartProperties
                                                                                    FilterSpec*
                                                                                    SortSpec*
                                                                                    (enum) ChartHiddenDimensionStrategy
                                                                                    BasicChartSpec*
                                                                                    PieChartSpec*
                                                                                    BubbleChartSpec*
                                                                                    CandlestickChartSpec*
                                                                                    OrgChartSpec*
                                                                                    HistogramChartSpec*
                                                                                    WaterfallChartSpec*
                                                                                    TreemapChartSpec*
                                                                                    ScorecardChartSpec*

                                            EmbeddedObjectPosition                  OverlayPosition                             GridCoordinate

                                            EmbeddedObjectBorder                    ColorStyle*

                    BandedRange             GridRange
                                            BandingProperties                       ColorStyle*

                    DeveloperMetadata       DeveloperMetadataLocation*
                                            (enum) DeveloperMetadataVisibility

                    DimensionGroup          DimensionRange*

                    Slicer                  SlicerSpec                              GridRange
                                                                                    FilterCriteria*
                                                                                    TextFormat
                                                                                    ColorStyle*
                                                                                    (enum) HorizontalAlign

                                            EmbeddedObjectPosition                  OverlayPosition
                                                                                    GridCoordinate*
 * 
*/
namespace GSHEET
{



}
#endif
