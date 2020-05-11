import React, { useState } from 'react';
import { View, StyleSheet, Dimensions } from 'react-native';

import ScreenContainer from '../components/ScreenContainer';
import Chart from '../components/Chart';
// import useInterval from '../utils/useInterval';

const width = Dimensions.get('window').width;
const height = Math.floor((Dimensions.get('window').height - 200) / 3);
// let count = 1;

const ChartsScreen = () => {
  const [data, setData] = useState([0, 0, 0, 0, 1, 10, 50]);
  const slotsPerWidth = 100;

  /*
  useInterval(() => {
    let newData = [];
    if (count <= slotsPerWidth) {
      newData = [].concat(data);
      newData.push(Math.floor(Math.random() * 100));
      count++;
    } else {
      newData.push(data[data.length - 1]);
      count = 1;
    }
    setData(newData);
  }, 100);
  */

  return (
    <ScreenContainer>
      <View style={styles.main}>
        <Chart
          data={data}
          maxValue={100}
          minValue={-100}
          slotsPerWidth={slotsPerWidth}
          width={width}
          height={height}
          lineColor="rgba(95, 92, 1, 1)"
          lineThickness={4}
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
};

const styles = StyleSheet.create({
  main: {
    flex: 1,
  },
});

export default ChartsScreen;
