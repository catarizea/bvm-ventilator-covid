import React from 'react';
import PropTypes from 'prop-types';
import { Svg, Polyline, Rect } from 'react-native-svg';

const convertArrayToPolylinePoints = ({
  data,
  height,
  slotWidth,
  maxValue,
  minValue,
}) => {
  let polylinePoints = '';

  for (let i = 0; i < data.length; i++) {
    let Y;

    if (data[i] === 0) {
      Y = height;
    } else {
      Y = Math.floor(
        height - (height * (data[i] - minValue)) / (maxValue - minValue),
      );
    }

    polylinePoints += `${slotWidth * i},${Y}`;

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
    fillColor,
    slotsPerWidth,
    chartBackground,
  } = props;

  const slotWidth = Math.floor(width / slotsPerWidth);

  const polylinePoints = convertArrayToPolylinePoints({
    data,
    height,
    slotWidth,
    maxValue,
    minValue,
  });

  return (
    <Svg width={width} height={height}>
      <Rect x="0" y="0" width={width} height={height} fill={chartBackground} />
      <Polyline
        points={polylinePoints}
        fill={fillColor}
        stroke={lineColor}
        strokeWidth={lineThickness}
      />
    </Svg>
  );
};

Chart.propTypes = {
  data: PropTypes.arrayOf(PropTypes.number.isRequired).isRequired,
  maxValue: PropTypes.number.isRequired,
  minValue: PropTypes.number.isRequired,
  slotsPerWidth: PropTypes.number.isRequired,
  width: PropTypes.number.isRequired,
  height: PropTypes.number.isRequired,
  fillColor: PropTypes.string.isRequired,
  lineColor: PropTypes.string.isRequired,
  lineThickness: PropTypes.number.isRequired,
  chartBackground: PropTypes.string.isRequired,
};

export default Chart;
