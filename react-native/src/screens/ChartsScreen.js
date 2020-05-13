import React, { Component } from 'react';
import { View, StyleSheet, Dimensions } from 'react-native';
import PropTypes from 'prop-types';

import ScreenContainer from '../components/ScreenContainer';
import Chart from '../components/Chart';

const width = Dimensions.get('window').width;
const height = Math.floor((Dimensions.get('window').height - 150) / 3);

let counter = 0;
const slotsPerWidth = 100;

class ChartsScreen extends Component {
  state = {
    chartData: {
      flow: [],
      volume: [],
      pressure: [],
    },
  };

  static getDerivedStateFromProps(props, state) {
    const { chartData } = state;
    const { sensorData } = props;

    if (sensorData.length && sensorData.length === 3) {
      if (!chartData.flow.length) {
        counter++;
        return {
          chartData: {
            flow: [sensorData[0]],
            volume: [sensorData[1]],
            pressure: [sensorData[2]],
          },
        };
      } else if (counter < slotsPerWidth) {
        counter++;
        return {
          chartData: {
            flow: [...chartData.flow, sensorData[0]],
            volume: [...chartData.volume, sensorData[1]],
            pressure: [...chartData.pressure, sensorData[2]],
          },
        };
      } else {
        counter = 2;
        return { 
          chartData: {
            flow: [chartData.flow[chartData.flow.length - 1], sensorData[0]],
            volume: [
              chartData.volume[chartData.volume.length - 1],
              sensorData[1],
            ],
            pressure: [
              chartData.pressure[chartData.pressure.length - 1],
              sensorData[2],
            ],
          },
        };
      }
    } else {
      return null;
    }
  }

  render() {
    const { flow, volume, pressure } = this.state.chartData;

    return (
      <ScreenContainer>
        <View style={styles.main}>
          <Chart
            key="flow"
            data={flow}
            maxValue={100}
            minValue={0}
            slotsPerWidth={slotsPerWidth}
            width={width}
            height={height}
            marginBottom={20}
            lineColor="rgba(95, 92, 1, 1)"
            lineThickness={2}
            chartBackground="#17204d"
            horizontalGridLinesCount={5}
            gridColor="rgba(65, 95, 93, .4)"
            gridThickness={1}
            unit="ml"
            axisTooClose={10}
            labelsColor="rgba(255, 255, 255, 0.8)"
            labelsFontSize={12}
            marginLeft={50}
            labelsMarginLeft={15}
          />
          <Chart
            key="volume"
            data={volume}
            maxValue={100}
            minValue={0}
            slotsPerWidth={slotsPerWidth}
            width={width}
            height={height}
            marginBottom={20}
            lineColor="rgba(95, 92, 1, 1)"
            lineThickness={2}
            chartBackground="#17204d"
            horizontalGridLinesCount={5}
            gridColor="rgba(65, 95, 93, .4)"
            gridThickness={1}
            unit="ml"
            axisTooClose={10}
            labelsColor="rgba(255, 255, 255, 0.8)"
            labelsFontSize={12}
            marginLeft={50}
            labelsMarginLeft={15}
          />
          <Chart
            key="pressure"
            data={pressure}
            maxValue={100}
            minValue={0}
            slotsPerWidth={slotsPerWidth}
            width={width}
            height={height}
            marginBottom={20}
            lineColor="rgba(95, 92, 1, 1)"
            lineThickness={2}
            chartBackground="#17204d"
            horizontalGridLinesCount={5}
            gridColor="rgba(65, 95, 93, .4)"
            gridThickness={1}
            unit="ml"
            axisTooClose={10}
            labelsColor="rgba(255, 255, 255, 0.8)"
            labelsFontSize={12}
            marginLeft={50}
            labelsMarginLeft={15}
          />
        </View>
      </ScreenContainer>
    );
  }
}

const styles = StyleSheet.create({
  main: {
    flex: 1,
  },
});

ChartsScreen.propTypes = {
  sensorData: PropTypes.array.isRequired,
};

export default ChartsScreen;
