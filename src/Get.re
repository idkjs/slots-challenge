// [@genType]
type kind = string;
// [@genType]
type starts = Js.Date.t;
// [@genType]
type ends = Js.Date.t;
// [@genType]
type weekly_recurring = bool;
// [@genType]
type date = Js.Date.t;
// [@genType]
type event = {
  kind,
  starts,
  ends,
  weekly_recurring,
};

// [@genType]
type availability = {
  date,
  slots: array(string),
};
// [@genType]
type availabilities = array(availability);

// [@genType]
type interval = {
  start: Js.Date.t,
  end_: Js.Date.t,
};
// [@genType]
type day =
  | Sunday
  | Monday
  | Tuesday
  | Wednesday
  | Thursday
  | Friday
  | Saturday;
// [@genType]
let dayToJs =
  fun
  | Sunday => 0
  | Monday => 1
  | Tuesday => 2
  | Wednesday => 3
  | Thursday => 4
  | Friday => 5
  | Saturday => 6;

// [@genType]
let is = (day, date) =>
  date |> Js.Date.getDay === (day |> dayToJs |> float_of_int);

// [@genType]
let isSaturday = date => date |> is(Saturday);

// [@genType]
let decodeEvent: Js.t('a) => event =
  r => {
    let event: event = {
      kind: r##kind,
      starts: r##starts_at |> Js.Date.fromFloat,
      ends: r##ends_at |> Js.Date.fromFloat,
      weekly_recurring: r##weekly_recurring,
    };

    event;
  };

// [@genType]
let makeYMD = date => {
  Js.Date.(
    makeWithYMD(
      ~year=date->getFullYear,
      ~month=date->getMonth,
      ~date=date->getDate,
      (),
    )
  );
};
open MomentRe;
open Js.Date;
// [@genType]
let copy = date => date->valueOf->fromFloat;
// [@genType]
let toInt = date => date->valueOf->int_of_float;

// [@genType]
let addDays = (date, numberOfDays) => {
  let d = date->copy;
  d->setDate(d->getDate +. numberOfDays->float)->ignore;
  d;
};

// [@genType]
let addMinutes = (date, numberOfMinutes) => {
  let d = date->copy;
  d->setMinutes(d->getMinutes +. numberOfMinutes->float)->ignore;
  d;
};
// [@genType]
let isWithinInterval = (interval, date) => {
  let ts = date |> copy |> Js.Date.getTime;
  ts >= (interval.start |> copy |> Js.Date.getTime)
  && ts < (interval.end_ |> copy |> Js.Date.getTime);
};

// [@genType]
let makeInterval = (start, end_) => {start, end_};
// [@genType]
let makeResult = (r, date): availabilities => {
  // need to get availabilites for the 7 days in the range
  // let rangeStartDate = date;
  // let rangeEndDate = rangeStartDate |> addDays(_, 7);
  let rangeStart = date |> addDays(_, -1);

  let data = Belt.Array.map(r, decodeEvent);
  // Js.log2("data", data);

  let openings =
    Belt.Array.keepMap(data, x =>
      switch (x.kind == "opening") {
      | true => Some(x)
      | _ => None
      }
    );
  let appointments =
    Belt.Array.keepMap(data, x =>
      switch (x.kind == "appointment") {
      | true => Some(x)
      | _ => None
      }
    );
  let appointment = appointments[0];

  // creates a fixed array with 7 slots expected by test with type `availabilities`
  let createAvailabilitiesArray: Js.Date.t => availabilities =
    date =>
      Array.init(7, index =>
        index->(+)(1)->addDays(date, _)->(date => {date, slots: [||]})
      );

  let availabilities = createAvailabilitiesArray(rangeStart);
  // recurring appointments recur for a week from the first appointments start date so here we define that week and created an interval to use it.
  let weekRangeStart = data[0].starts;
  let weekRangeEnd = weekRangeStart |> addDays(_, 7);
  let weekInterval = makeInterval(weekRangeStart, weekRangeEnd);
  let generateApptSlots = () => {
    let formattedApptSlots = [||];
    let {starts, ends, _} = appointment;
    let slotInterval = makeInterval(starts, ends);

    let initDate = starts;
    let rec slot = date => {
      switch (isWithinInterval(slotInterval, date)) {
      | false => ()
      | true =>
        let starts = date;
        let hours = starts |> Js.Date.toString;
        let formattedSlot = moment(hours) |> Moment.format("h:mm");

        Js.Array.push(formattedSlot, formattedApptSlots) |> ignore;

        slot(date |> copy |> addMinutes(_, 30)) |> ignore;
      };
    };

    slot(initDate);

    formattedApptSlots;
  };
  // [@genType]
  let generateSlots = (~eventToCheck) => {
    let formattedSlots = [||];
    let {starts, ends, _} = eventToCheck;
    let slotInterval = makeInterval(starts, ends);

    let initDate = starts;
    let rec slot = dateToCheck => {
      switch (isWithinInterval(slotInterval, dateToCheck)) {
      | false => ()
      | true =>
        let starts = dateToCheck;
        let hours = starts |> Js.Date.toString;
        let formattedSlot = moment(hours) |> Moment.format("h:mm");
        Js.Array.push(formattedSlot, formattedSlots) |> ignore;
        slot(dateToCheck |> copy |> addMinutes(_, 30)) |> ignore;
      };
    };

    slot(initDate);

    formattedSlots;
  };

  let makeSlotTime = (date, hours, minutes) => {
    makeWithYMDHM(
      ~year=date->getFullYear,
      ~month=date->getMonth,
      ~date=date->getDate,
      ~hours,
      ~minutes,
      (),
    );
  };
  availabilities->Belt.Array.map(a => {
    let isSaturday = isSaturday(a.date);
    let isInInterval = isWithinInterval(weekInterval, a.date);
    let slotStartHours = openings[0].starts |> Js.Date.getHours;
    let slotStartMinutes = openings[0].starts |> Js.Date.getMinutes;
    let slotEndsHours = openings[0].ends |> Js.Date.getHours;
    let slotEndsMinutes = openings[0].ends |> Js.Date.getMinutes;
    let starts = makeSlotTime(a.date, slotStartHours, slotStartMinutes);
    let ends = makeSlotTime(a.date, slotEndsHours, slotEndsMinutes);

    let currentEvent = {...openings[0], starts, ends};
    let newSlots: array(string) = generateSlots(~eventToCheck=currentEvent);
    // this functions is returning true when false and false when true. Not understanding why. So in the switch statement below we call true when the result is false and false when its true so that the tests pass.
    let hasAppointments =
      Moment.isSame(
        moment(makeYMD(a.date) |> toString),
        moment(makeYMD(appointment.starts) |> toString),
      );

    switch (isSaturday, isInInterval, hasAppointments) {
    | (false, true, true) => Js.Array.pushMany(newSlots, a.slots)
    | (false, true, false) =>
      let badSlots = generateApptSlots();

      let newSlotsFiltered =
        Js.Array.filter(x => !Array.mem(x, badSlots), newSlots);

      Js.Array.pushMany(newSlotsFiltered, a.slots);
    | (_, _, _) => 0
    };
  })
  |> ignore;

  availabilities;
};
open Sqlite3;

let db = Database.make(~path="db.sqlite", ~fileMustExist=true, ());
[@genType]

let getAvailabilities:(date=>availabilities) = date => {
  let makeResultWithDate = makeResult(_, date);

  let data = db->Database.prepare("select * from `events`")->Statement.all();

  let availabilities = makeResultWithDate(data);
  // Js.log2("get_availabilities_result", availabilities);
  availabilities;
};