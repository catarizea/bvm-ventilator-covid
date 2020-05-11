import React from 'react';
import { SafeAreaView, StyleSheet, View } from 'react-native';
import { Button } from 'react-native-paper';

import ScreenContainer from '../components/ScreenContainer';
import RangeSlider from '../components/RangeSlider';

const SettingsScreen = () => {
  const handlerRange = (value, fromUser, type) => {
    console.log(
      'range',
      `Type: ${type} Value: ${value} From User: ${fromUser}`,
    );
  };

  const handlerButton = (type) => {
    console.log('button', type);
  };

  return (
    <ScreenContainer>
      <>
        <View style={styles.spaceUp} />
        <SafeAreaView style={styles.main}>
          <RangeSlider
            key="volume"
            min={100}
            max={1600}
            step={50}
            initialLowValue={600}
            handler={(low, fromUser) => handlerRange(low, fromUser, 'volume')}
            label="Tidal Volume"
            unit="ml"
          />
          <RangeSlider
            key="rate"
            min={10}
            max={30}
            step={1}
            initialLowValue={15}
            handler={(low, fromUser) => handlerRange(low, fromUser, 'rate')}
            label="Respiratory Rate"
            unit="per minute"
          />
          <RangeSlider
            key="inspiration"
            min={1}
            max={5}
            step={1}
            initialLowValue={1}
            handler={(low, fromUser) => {
              handlerRange(low, fromUser, 'inspiration');
            }}
            label="Inspiration Ratio"
          />
          <RangeSlider
            key="expiration"
            min={1}
            max={5}
            step={1}
            initialLowValue={2}
            handler={(low, fromUser) => {
              handlerRange(low, fromUser, 'expiration');
            }}
            label="Expiration Ratio"
          />
        </SafeAreaView>
        <View style={styles.buttonContainer}>
          <Button
            icon="hand-left"
            mode="contained"
            style={[styles.button, styles.buttonStop]}
            onPress={() => handlerButton('stop')}
            labelStyle={styles.buttonLabel}>
            Stop
          </Button>
          <Button
            icon="air-filter"
            mode="contained"
            style={styles.button}
            onPress={() => handlerButton('start')}
            labelStyle={styles.buttonLabel}>
            Start
          </Button>
        </View>
      </>
    </ScreenContainer>
  );
};

const styles = StyleSheet.create({
  main: {
    flex: 6,
    justifyContent: 'center',
    alignItems: 'center',
  },
  spaceUp: {
    flex: 1,
  },
  buttonContainer: {
    flex: 2,
    justifyContent: 'space-around',
    alignItems: 'center',
    flexDirection: 'row',
  },
  button: {
    backgroundColor: '#25b35e',
  },
  buttonLabel: {
    fontSize: 25,
  },
  buttonStop: {
    backgroundColor: 'red',
  },
});

export default SettingsScreen;
