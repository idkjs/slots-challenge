/** 
 * this is a custom header you can add to every file!
 * TypeScript file generated from Get.re by genType.
 */
/* eslint-disable import/first */


const $$toRE17118703: { [key: string]: any } = {"Sunday": 0, "Monday": 1, "Tuesday": 2, "Wednesday": 3, "Thursday": 4, "Friday": 5, "Saturday": 6};

// tslint:disable-next-line:no-var-requires
const Curry = require('bs-platform/lib/es6/curry.js');

// tslint:disable-next-line:no-var-requires
const GetBS = require('./Get.bs');

import {Date_t as Js_Date_t} from './Js.gen';

// tslint:disable-next-line:interface-over-type-literal
export type kind = string;

// tslint:disable-next-line:interface-over-type-literal
export type starts = Js_Date_t;

// tslint:disable-next-line:interface-over-type-literal
export type ends = Js_Date_t;

// tslint:disable-next-line:interface-over-type-literal
export type weekly_recurring = boolean;

// tslint:disable-next-line:interface-over-type-literal
export type date = Js_Date_t;

export interface Ievent {
  readonly kind: kind; 
  readonly starts: starts; 
  readonly ends: ends; 
  readonly weekly_recurring: weekly_recurring
};

export interface Iavailability { readonly date: date; readonly slots: string[] };

// tslint:disable-next-line:interface-over-type-literal
export type availabilities = Iavailability[];

export interface Iinterval { readonly start: Js_Date_t; readonly end_: Js_Date_t };

// tslint:disable-next-line:interface-over-type-literal
export type day = 
    "Sunday"
  | "Monday"
  | "Tuesday"
  | "Wednesday"
  | "Thursday"
  | "Friday"
  | "Saturday";

export const dayToJs: (_1:day) => number = function (Arg1: any) {
  const result = GetBS.dayToJs($$toRE17118703[Arg1]);
  return result
};

export const is: (day:day, date:Js_Date_t) => boolean = function (Arg1: any, Arg2: any) {
  const result = Curry._2(GetBS.is, $$toRE17118703[Arg1], Arg2);
  return result
};

export const isSaturday: (date:Js_Date_t) => boolean = GetBS.isSaturday;

export const decodeEvent: (r:{
  readonly ends_at: number; 
  readonly kind: kind; 
  readonly starts_at: number; 
  readonly weekly_recurring: weekly_recurring
}) => Ievent = GetBS.decodeEvent;

export const makeYMD: (date:Js_Date_t) => Js_Date_t = GetBS.makeYMD;

export const copy: (date:Js_Date_t) => Js_Date_t = GetBS.copy;

export const toInt: (date:Js_Date_t) => number = GetBS.toInt;

export const addDays: (date:Js_Date_t, numberOfDays:number) => Js_Date_t = function (Arg1: any, Arg2: any) {
  const result = Curry._2(GetBS.addDays, Arg1, Arg2);
  return result
};

export const addMinutes: (date:Js_Date_t, numberOfMinutes:number) => Js_Date_t = function (Arg1: any, Arg2: any) {
  const result = Curry._2(GetBS.addMinutes, Arg1, Arg2);
  return result
};

export const isWithinInterval: (interval:Iinterval, date:Js_Date_t) => boolean = function (Arg1: any, Arg2: any) {
  const result = Curry._2(GetBS.isWithinInterval, Arg1, Arg2);
  return result
};

export const makeInterval: (start:Js_Date_t, end_:Js_Date_t) => Iinterval = function (Arg1: any, Arg2: any) {
  const result = Curry._2(GetBS.makeInterval, Arg1, Arg2);
  return result
};

export const makeResult: (r:Array<{
  readonly ends_at: number; 
  readonly kind: kind; 
  readonly starts_at: number; 
  readonly weekly_recurring: weekly_recurring
}>, date:Js_Date_t) => availabilities = function (Arg1: any, Arg2: any) {
  const result = Curry._2(GetBS.makeResult, Arg1, Arg2);
  return result
};

export const getAvailabilities: (date:Js_Date_t) => availabilities = GetBS.getAvailabilities;
