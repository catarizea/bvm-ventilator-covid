import React from 'react';
import PropTypes from 'prop-types';
import { NavigationContainer } from '@react-navigation/native';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import { Appbar, withTheme } from 'react-native-paper';
import Icon from 'react-native-vector-icons/MaterialCommunityIcons';
import Color from 'color';

import ChartsScreen from '../screens/ChartsScreen';
import SettingsScreen from '../screens/SettingsScreen';

const Tab = createBottomTabNavigator();

const Navigation = (props) => {
  const { theme, sensorData, writeNewSettings } = props;
  const primaryColor = Color(theme.colors.primary);
  const title = 'BVM Ventilator Covid-19';

  return (
    <NavigationContainer>
      <Tab.Navigator
        screenOptions={({ route }) => ({
          tabBarIcon: () => {
            let iconName;

            if (route.name === 'Charts') {
              iconName = 'chart-areaspline';
            } else if (route.name === 'Settings') {
              iconName = 'tune';
            }

            return (
              <Icon name={iconName} size={25} color={theme.colors.accent} />
            );
          },
        })}
        tabBarOptions={{
          activeTintColor: theme.colors.accent,
          inactiveTintColor: theme.colors.accent,
          activeBackgroundColor: primaryColor.darken(0.07).rgb(),
          inactiveBackgroundColor: theme.colors.primary,
          labelStyle: {
            fontSize: 15,
          },
          style: {
            borderTopWidth: 0,
          },
          tabStyle: {
            borderTopWidth: 0,
          },
        }}>
        <Tab.Screen name="Charts">
          {(screenProps) => (
            <>
              <Appbar.Header>
                <Appbar.Content title={`Charts | ${title}`} />
              </Appbar.Header>
              <ChartsScreen {...screenProps} sensorData={sensorData} />
            </>
          )}
        </Tab.Screen>
        <Tab.Screen name="Settings">
          {(screenProps) => (
            <>
              <Appbar.Header>
                <Appbar.Content title={`Settings | ${title}`} />
              </Appbar.Header>
              <SettingsScreen
                {...screenProps}
                writeNewSettings={writeNewSettings}
              />
            </>
          )}
        </Tab.Screen>
      </Tab.Navigator>
    </NavigationContainer>
  );
};

Navigation.propTypes = {
  sensorData: PropTypes.array.isRequired,
  writeNewSettings: PropTypes.func.isRequired,
};

export default withTheme(Navigation);
