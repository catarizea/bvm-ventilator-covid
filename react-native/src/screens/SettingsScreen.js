import React, { useState } from 'react';
import { SafeAreaView, StyleSheet, View } from 'react-native';
import { Button } from 'react-native-paper';
import PropTypes from 'prop-types';

import ScreenContainer from '../components/ScreenContainer';
import RangeSlider from '../components/RangeSlider';

const SettingsScreen = ({ writeNewSettings }) => {
  const [settings, setSettings] = useState({
    volume: 600,
    rate: 15,
    inspiration: 1,
    expiration: 2,
  });

  const handlerRange = (value, fromUser, type) => {
    const newSettings = { ...settings };
    const newValue = parseInt(value, 10);

    if (newSettings[type] === newValue) {
      return;
    }

    newSettings[type] = newValue;
    setSettings(newSettings);
  };

  const handlerButton = (type) => {
    if (type === 'stop') {
      writeNewSettings([0, 0, 0, 0]);
    }

    if (type === 'start') {
      writeNewSettings([
        settings.volume,
        settings.rate,
        settings.inspiration,
        settings.expiration,
      ]);
    }
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
            initialLowValue={settings.volume}
            handler={(low, fromUser) => handlerRange(low, fromUser, 'volume')}
            label="Tidal Volume"
            unit="ml"
            currentValue={settings.volume}
          />
          <RangeSlider
            key="rate"
            min={10}
            max={30}
            step={1}
            initialLowValue={settings.rate}
            handler={(low, fromUser) => handlerRange(low, fromUser, 'rate')}
            label="Respiratory Rate"
            unit="per minute"
            currentValue={settings.rate}
          />
          <RangeSlider
            key="inspiration"
            min={1}
            max={5}
            step={1}
            initialLowValue={settings.inspiration}
            handler={(low, fromUser) => {
              handlerRange(low, fromUser, 'inspiration');
            }}
            label="Inspiration Ratio"
            currentValue={settings.inspiration}
          />
          <RangeSlider
            key="expiration"
            min={1}
            max={5}
            step={1}
            initialLowValue={settings.expiration}
            handler={(low, fromUser) => {
              handlerRange(low, fromUser, 'expiration');
            }}
            label="Expiration Ratio"
            currentValue={settings.expiration}
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
    flex: 8,
    justifyContent: 'center',
    alignItems: 'center',
  },
  spaceUp: {
    flex: 1,
  },
  buttonContainer: {
    flex: 4,
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

SettingsScreen.propTypes = {
  writeNewSettings: PropTypes.func.isRequired,
};

export default SettingsScreen;
