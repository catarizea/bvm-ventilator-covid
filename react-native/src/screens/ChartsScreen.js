import React, { Component } from 'react';
import { View, StyleSheet, Dimensions } from 'react-native';
import PropTypes from 'prop-types';

import ScreenContainer from '../components/ScreenContainer';
import Chart from '../components/Chart';

const width = Dimensions.get('window').width;
const height = Math.floor((Dimensions.get('window').height - 200) / 3);

let counter = 0;
const slotsPerWidth = 10;

class ChartsScreen extends Component {
  state = {
    chartData: [],
  };

  static getDerivedStateFromProps(props, state) {
    const { chartData } = state;
    const { sensorData } = props;

    if (sensorData.length && sensorData.length === 3) {
      if (!chartData.length) {
        counter++;
        return { chartData: [sensorData[0]] };
      } else if (counter < slotsPerWidth) {
        counter++;
        return { chartData: [...chartData, sensorData[0]] };
      } else {
        counter = 2;
        return { chartData: [chartData[chartData.length - 1], sensorData[0]] };
      }
    } else {
      return null;
    }
  }

  render() {
    return (
      <ScreenContainer>
        <View style={styles.main}>
          <Chart
            data={this.state.chartData}
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
