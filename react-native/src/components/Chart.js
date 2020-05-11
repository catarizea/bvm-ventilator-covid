import React from 'react';
import PropTypes from 'prop-types';
import { View, StyleSheet } from 'react-native';
import { Svg, Polyline, Rect } from 'react-native-svg';

import Grid from './Grid';

const convertArrayToPolylinePoints = ({
  data,
  height,
  slotWidth,
  maxValue,
  minValue,
  marginLeft,
}) => {
  let polylinePoints = '';

  if (!data.length) {
    return polylinePoints;
  }

  for (let i = 0; i < data.length; i++) {
    const Y = Math.floor(
      height -
        (height * Math.abs(data[i] - minValue)) / Math.abs(maxValue - minValue),
    );

    polylinePoints += `${slotWidth * i + marginLeft},${Y}`;

    if (i < data.length - 1) {
      polylinePoints += ' ';
    }
  }

  return polylinePoints;
};

const Chart = (props) => {
  const {
    data,
    maxValue,
    minValue,
    width,
    height,
    lineColor,
    lineThickness,
    slotsPerWidth,
    chartBackground,
    horizontalGridLinesCount,
    gridColor,
    gridThickness,
    unit,
    axisTooClose,
    labelsColor,
    labelsFontSize,
    marginLeft,
    labelsMarginLeft,
  } = props;

  const slotWidth = (width - marginLeft) / slotsPerWidth;

  const polylinePoints = convertArrayToPolylinePoints({
    data,
    height,
    slotWidth,
    maxValue,
    minValue,
    marginLeft,
  });

  return (
    <View style={styles.container}>
      <Svg width={width} height={height}>
        <Rect
          x="0"
          y="0"
          width={width}
          height={height}
          fill={chartBackground}
        />
        <Polyline
          points={polylinePoints}
          stroke={lineColor}
          strokeWidth={lineThickness}
        />
        <Grid
          gridCount={horizontalGridLinesCount}
          marginLeft={marginLeft}
          maxValue={maxValue}
          minValue={minValue}
          width={width}
          height={height}
          gridColor={gridColor}
          gridThickness={gridThickness}
          unit={unit}
          axisTooClose={axisTooClose}
          labelsColor={labelsColor}
          labelsFontSize={labelsFontSize}
          labelsMarginLeft={labelsMarginLeft}
        />
      </Svg>
    </View>
  );
};

Chart.propTypes = {
  data: PropTypes.arrayOf(PropTypes.number).isRequired,
  maxValue: PropTypes.number.isRequired,
  minValue: PropTypes.number.isRequired,
  slotsPerWidth: PropTypes.number.isRequired,
  width: PropTypes.number.isRequired,
  height: PropTypes.number.isRequired,
  lineColor: PropTypes.string.isRequired,
  lineThickness: PropTypes.number.isRequired,
  chartBackground: PropTypes.string.isRequired,
  horizontalGridLinesCount: PropTypes.number.isRequired,
  gridColor: PropTypes.string.isRequired,
  gridThickness: PropTypes.number.isRequired,
  unit: PropTypes.string.isRequired,
  axisTooClose: PropTypes.number.isRequired,
  labelsColor: PropTypes.string.isRequired,
  labelsFontSize: PropTypes.number.isRequired,
  marginLeft: PropTypes.number.isRequired,
  labelsMarginLeft: PropTypes.number.isRequired,
};

const styles = StyleSheet.create({
  flex: 1,
});

export default Chart;
