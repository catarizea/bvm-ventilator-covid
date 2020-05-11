import React from 'react';
import { Svg, Line, Text } from 'react-native-svg';
import PropTypes from 'prop-types';

const Grid = (props) => {
  const {
    width,
    height,
    gridCount,
    marginLeft,
    maxValue,
    minValue,
    gridColor,
    gridThickness,
    unit,
    axisTooClose,
    labelsColor,
    labelsFontSize,
    labelsMarginLeft,
  } = props;

  const spaceBetweenHorizontalGridLines = Math.floor(height / (gridCount + 1));
  const deltaValuesBetweenHorizontalGridLines = Math.floor(
    Math.abs(maxValue - minValue) / (gridCount + 1),
  );

  const gridLines = [];
  const gridLabels = [];

  // add 0 axis
  const zeroY =
    height - (height * Math.abs(minValue)) / Math.abs(maxValue - minValue);

  gridLines.push(
    <Line
      key="grd-0-axis"
      x1={marginLeft}
      y1={zeroY}
      x2={width}
      y2={zeroY}
      stroke={gridColor}
      strokeWidth={gridThickness}
    />,
  );

  gridLabels.push(
    <Text
      fill={labelsColor}
      fontSize={labelsFontSize}
      textAnchor="middle"
      x={labelsMarginLeft}
      y={zeroY + labelsFontSize * 0.3}>
      0 {unit}
    </Text>,
  );

  gridLabels.push();

  if (gridCount > 1) {
    for (let i = 0; i < gridCount; i++) {
      const currentDataLabel =
        minValue + (i + 1) * deltaValuesBetweenHorizontalGridLines;

      // do not render if it's too close to 0 axis
      if (Math.abs(currentDataLabel) < axisTooClose) {
        continue;
      }

      const Y = height - (i + 1) * spaceBetweenHorizontalGridLines;

      gridLines.push(
        <Line
          key={`grd-${i}`}
          x1={marginLeft}
          y1={Y}
          x2={width}
          y2={Y}
          stroke={gridColor}
          strokeWidth={gridThickness}
        />,
      );

      gridLabels.push(
        <Text
          fill={labelsColor}
          fontSize={labelsFontSize}
          textAnchor="middle"
          x={labelsMarginLeft}
          y={Y + labelsFontSize * 0.3}>
          {currentDataLabel} {unit}
        </Text>,
      );
    }
  }

  return (
    <Svg width={width} height={height}>
      {gridLines}
      {gridLabels}
    </Svg>
  );
};

Grid.propTypes = {
  gridCount: PropTypes.number.isRequired,
  marginLeft: PropTypes.number.isRequired,
  maxValue: PropTypes.number.isRequired,
  minValue: PropTypes.number.isRequired,
  width: PropTypes.number.isRequired,
  height: PropTypes.number.isRequired,
  gridColor: PropTypes.string.isRequired,
  gridThickness: PropTypes.number.isRequired,
  unit: PropTypes.string.isRequired,
  axisTooClose: PropTypes.number.isRequired,
  labelsColor: PropTypes.string.isRequired,
  labelsFontSize: PropTypes.number.isRequired,
  labelsMarginLeft: PropTypes.number.isRequired,
};

export default Grid;
